#pragma once
#include "../Engine/Timer.h"
#include "Fish.h"

class FishGenerator
{
public:
	FishGenerator();
	void GenerateFish(double dt);
	//void DeleteFish();

	//std::vector<Fish*> fishList;

private:
	CS230::Timer* timer;
};