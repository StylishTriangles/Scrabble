#ifndef TIMEREVENT_H
#define TIMEREVENT_H

#include "event.h"

class TimerEvent : public Event
{
public:
    TimerEvent(EventType t, int timerID);
    int timerID() const {return _timerID;}
private:
    const int _timerID;
};

#endif // TIMEREVENT_H
