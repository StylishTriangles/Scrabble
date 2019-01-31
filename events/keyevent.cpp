#include "keyevent.h"

/**
 *  @brief Construct KeyEvent object.
 *	@param t: Enumerated value.
 *	@param keyCode: Virtual key code.
 **/
KeyEvent::KeyEvent(EventType t, short keyCode) :
    Event(t),
    virtualKeyCode(keyCode)
{

}

/**
 *  @brief Return event's virtual key code.
 **/
const short& KeyEvent::key() const
{
    return virtualKeyCode;
}
