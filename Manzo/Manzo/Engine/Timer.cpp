/*
Copyright (C) 2023 DigiPen Institute of Technology
Reproduction or distribution of this file or its contents without
prior written consent is prohibited
File Name:  Timer.cpp
Project:    CS230 Engine
Author:     Jonathan Holmes, Seohyeon Min
Created:    May 15, 2023
*/

#include "Timer.h"

CS230::Timer::Timer(double time_remaining)
{
	Set(time_remaining);
}

void CS230::Timer::Set(double time_remaining)
{
	timer_max = time_remaining;
	pendulum = false;
	Reset();
}

void CS230::Timer::Update(double dt)
{
	pendulum = !pendulum;
	if (timer >= 0) {
		timer -= dt;
	}
	else {
		timer = 0.0;
		pendulum = false;
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
	return (int)timer;
}

bool CS230::Timer::TickTock()
{
	return pendulum;
}



/// <summary>
/// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// </summary>
/// <param name="duration_sec"></param>



CS230::RealTimeTimer::RealTimeTimer(double duration_sec) {
    Set(duration_sec);
}

void CS230::RealTimeTimer::Set(double duration_sec) {
    duration = duration_sec;
    pendulum = false;
    Reset();
}

void CS230::RealTimeTimer::Update() {
    pendulum = !pendulum;
    auto now = std::chrono::high_resolution_clock::now();
    double elapsed = std::chrono::duration<double>(now - start_time).count();

    if (elapsed >= duration) {
        auto overshoot = std::chrono::duration_cast<std::chrono::high_resolution_clock::duration>(
            std::chrono::duration<double>(elapsed - duration)
        );
        start_time = now - overshoot; // 타입 일치 보장
        pendulum = false;
    }
}

void CS230::RealTimeTimer::Reset() {
    start_time = std::chrono::high_resolution_clock::now();
}

double CS230::RealTimeTimer::Remaining() {
    auto now = std::chrono::high_resolution_clock::now();
    double elapsed = std::chrono::duration<double>(now - start_time).count();
    return (elapsed >= duration) ? 0.0 : (duration - elapsed);
}

int CS230::RealTimeTimer::RemainingInt() {
    return static_cast<int>(Remaining());
}

bool CS230::RealTimeTimer::TickTock() {
    return pendulum;
}