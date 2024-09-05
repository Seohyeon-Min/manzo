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
