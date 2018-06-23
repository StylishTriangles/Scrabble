#include "timerevent.h"

TimerEvent::TimerEvent(EventType t, int timerID) :
    Event(t), _timerID(timerID)
{}
