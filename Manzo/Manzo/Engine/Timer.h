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
//#include "Fonts.h"

#pragma once

class Timer : public Component {
public:
    Timer(double time_remaining);
    void Set(double time_remaining);
    void Update(double dt) override;
    void Reset();
    double Remaining() const;
    int RemainingInt();
    bool TickTock();
    bool IsFinished() const;
private:
    double timer;
    double timer_max;
    bool pendulum;
};

class RealTimeTimer : public Component {
public:
    RealTimeTimer(double duration);
    void Set(double duration);
    void Start();
    void Pause();
    void Reset();
    double Remaining() const;
    bool IsRunning() const;
    bool IsFinished() const;

private:
    double duration; // 타이머 지속 시간 (초)
    bool running;    // 타이머 실행 상태
    std::chrono::time_point<std::chrono::steady_clock> start_time;
    double paused_time; // 일시 정지된 상태에서의 남은 시간
};

