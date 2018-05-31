#include "eventmanager.h"
#ifdef WIN32
#include <Windows.h>
#else
#error Non Windows EventManager has not been implemented yet.
#endif

std::queue<Event*> EventManager::eq;
std::mutex EventManager::eqLock;
std::multimap<EventType, std::function< void(Event*) > > EventManager::cbs;
std::mutex EventManager::cbsLock;
std::atomic<bool> EventManager::started;
std::atomic<bool> EventManager::terminate;
std::thread *EventManager::io;
std::mutex EventManager::threadManager;

void EventManager::connect(EventType et, std::function< void(Event *) > callback)
{
    cbsLock.lock();
    cbs.insert({et, callback});
    cbsLock.unlock();
}

void EventManager::pollEvents()
{
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
}

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
    threadManager.unlock();
}

void EventManager::scanEvents()
{
    bool keyState[0xFF] = {};
    while (!terminate.load()) {
        eqLock.lock();
        // keyboard
#ifdef WIN32
        for (int i = 8; i < 0xFF; i++) { // start from 8 to skip mouse buttons
            bool ks = ((unsigned short)GetKeyState(i))&(1<<15);
            if (ks & !keyState[i])
                eq.push(new KeyEvent(KeyPress, (short)i));
            else if (!ks & keyState[i])
                eq.push(new KeyEvent(KeyRelease, (short)i));
            keyState[i] = ks;
        }
#endif
        eqLock.unlock();
    }
}
