#ifndef EVENT_H
#define EVENT_H

enum EventType {
    KeyPress,
    KeyRelease,
    Timer,
};

class Event
{
public:
    Event(EventType t);
    virtual ~Event() = default;
    EventType type() {return et;}
protected:
    EventType et;
};

#endif // EVENT_H
