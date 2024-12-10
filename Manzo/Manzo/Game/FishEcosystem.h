#pragma once
#include "../Engine/Timer.h"
#include "Fish.h"
#include "AI.h"

static Quadtree quadtree(AABB{ {-Engine::window_width, -Engine::window_height}, {Engine::window_width, Engine::window_height} });

class FishGenerator
{
public:
	FishGenerator();
	void GenerateFish(double dt);
	~FishGenerator();

	std::vector<Fish*> fishList;

private:
	Timer* timer;

	struct Formation {
		std::vector<vec2> offsets;
		float randomOffsetMinX, randomOffsetMaxX;
		float randomOffsetMinY, randomOffsetMaxY;  
	};

    const std::vector<Formation> formations = {
        {
            { {-30, -30}, {-30, 30}, {-60, -60}, {-60, 60} }, // Formation original
            70.f, 100.f, 80.f, 110.f
        },
        {
            { {-30, -30}, {-30, 30} }, // Formation triangle
            70.f, 80.f, 70.f, 80.f 
        },
        {
            { {-30, -30}, {-30, 30}, {-60, 0} }, // Formation diamond
            70.f, 80.f, 70.f, 80.f 
        }
    };
};