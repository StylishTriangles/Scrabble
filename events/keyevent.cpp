#include "keyevent.h"

KeyEvent::KeyEvent(EventType t, short keyCode) :
    Event(t),
    virtualKeyCode(keyCode)
{

}

const short& KeyEvent::key() const
{
    return virtualKeyCode;
}
