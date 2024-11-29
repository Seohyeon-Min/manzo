#pragma once
#include "../Engine/Timer.h"
#include "Fish.h"

class FishGenerator
{
public:
	FishGenerator();
	void GenerateFish(double dt);
	~FishGenerator();

	std::vector<Fish*> fishList;

private:
	CS230::Timer* timer;
	CS230::Timer* bg_timer;
	float height = Engine::window_height;
};