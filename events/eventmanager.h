#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include <atomic>
#include <functional>
#include <map>
#include <mutex>
#include <queue>
#include <thread>

#include "event.h"
#include "keyevent.h"

class EventManager;

class EventManager
{
public:
//    static void connect(EventType et, void (*callback)(Event *));
    static void connect(EventType et, std::function< void(Event*) > callback);
    static void pollEvents();
    static void start();
    static void stop();
private:
    static void scanEvents();
    static std::queue<Event*> eq;
    static std::mutex eqLock;
    static std::multimap<EventType, std::function< void(Event*) > > cbs;
    static std::mutex cbsLock;
    static std::atomic<bool> started, terminate;
    static std::thread *io;
    static std::mutex threadManager;
};

#endif // EVENTMANAGER_H
