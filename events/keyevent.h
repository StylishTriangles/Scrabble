#ifndef KEYEVENT_H
#define KEYEVENT_H

#include "event.h"

class KeyEvent : public Event
{
public:
    KeyEvent(EventType t, short keyCode);
    const short &key() const;
private:
    short virtualKeyCode;
};

#endif // KEYEVENT_H
