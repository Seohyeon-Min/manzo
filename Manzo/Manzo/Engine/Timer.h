/*
Copyright (C) 2023 DigiPen Institute of Technology
Reproduction or distribution of this file or its contents without
prior written consent is prohibited
File Name:  Timer.h
Project:    CS230 Engine
Author:     Seohyeon Min
Updated:    May 12, 2023
*/


#include "Component.h"
#include "Engine.h"

#include <chrono>

#pragma once
namespace CS230 {
    class Timer : public Component {
    public:
        Timer(double time_remaining);
        void Set(double time_remaining);
        void Update(double dt) override;
        void Reset();
        double Remaining();
        int RemainingInt();
        bool TickTock();
    private:
        double timer;
        double timer_max;
        bool pendulum;
    };

    class RealTimeTimer : public Component {
    public:
        RealTimeTimer(double duration_sec);
        void Set(double duration_sec);
        void Update();
        void Reset();
        double Remaining();
        int RemainingInt();
        bool TickTock();

    private:
        double duration; // 타이머 최대 시간 (초 단위)
        std::chrono::time_point<std::chrono::high_resolution_clock> start_time; // 시작 시간
        bool pendulum;   // 진자 상태
    };
}
