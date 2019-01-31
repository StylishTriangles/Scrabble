#include "etimer.h"

/**
 *  @brief Construct ETimer object.
 *	@param dT: delta time between timeouts.
 **/
ETimer::ETimer(std::chrono::milliseconds dT) :
    dT(dT), until(std::chrono::system_clock::now() + dT)
{
    static int enumID = 0;
    enumID++;
    ID = enumID;
}

/**
 *  Return true when timer times out, in this case also reset timer.
 **/
bool ETimer::test(std::chrono::system_clock::time_point now)
{
    if (now > until) {
        until += dT;
        return true;
    }
    return false;
}
