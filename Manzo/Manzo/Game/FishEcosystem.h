#pragma once
#include "../Engine/Timer.h"
#include "Fish.h"
#include "FlockingFish.h"
#include "Ship.h"

#include <random>

class FishGenerator : public Component
{
public:
	FishGenerator();
    ~FishGenerator();

    void ReadFishCSV(const std::string& filename);
	void GenerateFish(double dt);

    int GetMoney() { return money; }
    void ClearMoney() { money = 0; }
    void SetMoney(int count) { money = count; }
    int ReturnFishMoney(int index);

    void SetFishCnt(int n) { fishCnt = n; }
    int GetFishCnt() { return fishCnt; }

    enum FishType
    {
		Fish1 = 1, Fish2, Fish3, Fish4, Fish5, Fish6, Fish7, Fish8
    };

    struct FishDex
    {
        FishType type;
        vec2 scale;
        vec2 velocity;
        std::filesystem::path filePath;
        float possibility;
        int money;
    };

	std::vector<Fish*> fishList;
    std::vector<int> moneys;
    std::vector<float> weights;
    std::vector<FishDex> fishBook;


private:
    int money;
    int fishCnt;
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

    friend class Fish;
    Ship* ship;
};