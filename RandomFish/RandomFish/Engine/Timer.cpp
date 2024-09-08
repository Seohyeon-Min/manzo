#include "Timer.h"
#include <iostream>

CS230::Timer::Timer(double time_remaining)
{
    Set(time_remaining);
}

void CS230::Timer::Set(double time_remaining)
{
    timer_max = time_remaining;
    Reset();
    pendulum = false;
}

void CS230::Timer::Update(double dt) {
    if (timer > 0)
    {
        timer -= dt;
        pendulum = !pendulum;
        if (timer < 0)
        {
            timer = 0;
        }
    }
}

void CS230::Timer::Reset()
{
    timer = timer_max;
}

double CS230::Timer::Remaining()
{
    return timer;
}

int CS230::Timer::RemainingInt()
{
    return static_cast<int>(timer);
}

bool CS230::Timer::TickTock()
{
    return pendulum;
}
