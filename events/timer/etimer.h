#ifndef TIMERER_H
#define TIMERER_H

#include <chrono>

class ETimer;

class ETimer
{
public:
    ETimer(std::chrono::milliseconds dT);
    int getID() const {return ID;}
    bool test (std::chrono::system_clock::time_point now = std::chrono::system_clock::now());

private:
    int ID;
    std::chrono::milliseconds dT;
    std::chrono::system_clock::time_point until;
};

#endif // TIMERER_H
