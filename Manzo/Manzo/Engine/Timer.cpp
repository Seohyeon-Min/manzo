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

Timer::Timer(double time_remaining)
{
	Set(time_remaining);
}

void Timer::Set(double time_remaining)
{
	timer_max = time_remaining;
	pendulum = false;
	Reset();
}

void Timer::Update(double dt)
{
    if (pendulum_cnt >= 12) {
        pendulum = !pendulum;
        pendulum_cnt = 0;
    }

    pendulum_cnt++;

	if (timer >= 0) {
		timer -= dt;
	}
	else {
		timer = 0.0;
		pendulum = false;
	}
}

void Timer::Reset()
{
	timer = timer_max;
}

double Timer::Remaining() const
{
	return timer;
}

int Timer::RemainingInt()
{
	return (int)timer;
}

bool Timer::TickTock()
{
	return pendulum;
}

bool Timer::IsFinished() const
{
    return Remaining() <= 0.0;
}



RealTimeTimer::RealTimeTimer(double duration)
    : duration(duration), running(false), paused_time(0) {}

void RealTimeTimer::Set(double duration) {
    this->duration = duration;
    Reset();
}

void RealTimeTimer::Start() {
    if (!running) {
        running = true;
        start_time = std::chrono::steady_clock::now();
    }
}

void RealTimeTimer::Pause() {
    if (running) {
        running = false;
        auto now = std::chrono::steady_clock::now();
        double elapsed = std::chrono::duration<double>(now - start_time).count();
        paused_time = std::max(0.0, duration - elapsed);
    }
}

void RealTimeTimer::Reset() {
    running = false;
    paused_time = duration;
}

double RealTimeTimer::Remaining() const {
    if (!running) {
        return paused_time;
    }
    auto now = std::chrono::steady_clock::now();
    double elapsed = std::chrono::duration<double>(now - start_time).count();
    return std::max(0.0, duration - elapsed);
}

bool RealTimeTimer::IsRunning() const {
    return running;
}

bool RealTimeTimer::IsFinished() const {
    return Remaining() <= 0.0;
}