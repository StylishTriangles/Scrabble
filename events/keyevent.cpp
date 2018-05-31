#include "keyevent.h"

KeyEvent::KeyEvent(EventType t, short keyCode) :
    Event(t),
    virtualKeyCode(keyCode)
{

}

short KeyEvent::key()
{
    return virtualKeyCode;
}
