#include "Component.h"
#pragma once

namespace CS230 {
    class Timer : public Component{
    public:
        Timer(double time_remaining);
        void Set(double time_remaining);
        void Update(double dt) override;
        void Reset();
        double Remaining();
        int RemainingInt();
        bool TickTock();

        double timer;
        double timer_max;
        bool pendulum;

    private:
    };
}
