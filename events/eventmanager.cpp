#include "eventmanager.h"
#ifdef WIN32
#include <windows.h>
#else
#error Non Windows EventManager has not been implemented yet.
#endif

std::queue<Event*> EventManager::eq;
std::mutex EventManager::eqLock;
std::multimap<EventType, std::function< void(Event*) > > EventManager::cbs;
std::mutex EventManager::cbsLock;
std::map<int, ETimer* > EventManager::timers;
std::mutex EventManager::timersLock;
std::atomic<bool> EventManager::started;
std::atomic<bool> EventManager::terminate;
std::thread *EventManager::io;
std::mutex EventManager::threadManager;

/**
 *  @brief Register callback for a specific event type.
 *  @param et: EventType to capture.
 *	@param callback: callback to call when specified event happens.
 **/
void EventManager::connect(EventType et, std::function< void(Event *) > callback)
{
    cbsLock.lock();
    cbs.insert({et, callback});
    cbsLock.unlock();
}

/**
 *  @brief Register a new timer.
 *  @param dT: Time delta between subsequent calls.
 *	@param callback: Callback to call every dT milliseconds.
 **/
int EventManager::newTimer(std::chrono::milliseconds dT, std::function<void (TimerEvent *)> callback)
{
    ETimer *t = new ETimer(dT);
    int tID = t->getID();
    auto wrapper = [callback, tID](Event* e){
        TimerEvent *e2 = dynamic_cast<TimerEvent *>(e);
        if (tID == e2->timerID()) callback(e2); // only exec when timerID matches this timer's id
    };
    timersLock.lock();
    timers.insert(std::make_pair(tID, t));
    timersLock.unlock();
    cbsLock.lock();
    cbs.insert({EventType::Timer, wrapper});
    cbsLock.unlock();
    return tID;
}

/**
 *  @brief Execute callbacks for all events that happen since last call to this function.
 **/
void EventManager::pollEvents()
{
    threadManager.lock();
    eqLock.lock();
    cbsLock.lock();
    while (!eq.empty()) {
        Event* e = eq.front();
        auto ret = cbs.equal_range(e->type());
        for (auto it = ret.first; it != ret.second; ++it)
            it->second(e);
        delete e;
        eq.pop();
    }
    cbsLock.unlock();
    eqLock.unlock();
    threadManager.unlock();
}

/**
 *  @brief Start the event manager.
 **/
void EventManager::start()
{
    threadManager.lock();
    if (started.load()) {
        threadManager.unlock();
        return;
    }

    started.store(true);
    terminate.store(false);
    io = new std::thread([](){
        scanEvents();
    });
    threadManager.unlock();
}

/**
 *  @brief Stop the event manager.
 **/
void EventManager::stop()
{
    threadManager.lock();
    if (!started.load()) {
        threadManager.unlock();
        return;
    }

    terminate.store(true);
    io->join();
    delete io;
    started.store(false);
    cleanup();
    threadManager.unlock();
}

void EventManager::scanEvents()
{
    bool keyState[0xFF] = {};
    while (!terminate.load()) {
        // keyboard events
#ifdef WIN32
        for (int i = 8; i < 0xFF; i++) { // start from 8 to skip mouse buttons
            bool ks = static_cast<unsigned short>(GetKeyState(i))&(1<<15);
            if (ks & !keyState[i]) {
                eqLock.lock();
                eq.push(new KeyEvent(KeyPress, short(i)));
                eqLock.unlock();
            }
            else if (!ks & keyState[i]) {
                eqLock.lock();
                eq.push(new KeyEvent(KeyRelease, short(i)));
                eqLock.unlock();
            }
            keyState[i] = ks;
        }
#endif
        // Timer events
        auto now = std::chrono::system_clock::now();
        timersLock.lock();
        for (auto & it: timers) {
            if (it.second->test(now)) {
                eqLock.lock();
                eq.push(new TimerEvent(Timer, it.second->getID()));
                eqLock.unlock();
            }
        }
        timersLock.unlock();
    }
}

void EventManager::cleanup()
{
    eqLock.lock();
    cbsLock.lock();
    timersLock.lock();
    while (!eq.empty()) {
        delete eq.front();
        eq.pop();
    }
    cbs.clear();
    for (auto & it: timers) {
        delete it.second;
    }
    timers.clear();
    eqLock.unlock();
    cbsLock.unlock();
    timersLock.unlock();
}
