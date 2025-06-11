#pragma once

#include "../Engine/GameObject.h"
#include "GameObjectTypes.h"
#include "../Engine/MapManager.h"
#include "Ship.h"
#include <map>

extern std::map<int, int> fishCollection;

class Fish : public GameObject
{
public:
	Fish(int index) : Fish(index, nullptr) { };
	Fish(int index, Fish* parent);
	GameObjectTypes Type() override { return GameObjectTypes::Fish; }
	std::string TypeName() override { return "Fish"; }
	bool CanCollideWith(GameObjectTypes) override;
	void ResolveCollision([[maybe_unused]] GameObject* other_object) override;

	vec2 start_position = { 0,0 };

	int type;
	bool flag = false;
	bool collided = false;

	void Update(double dt) override;
	void Draw();
	void Pattern();
	void AvoidRock();

private:
	float RandomFloat(float min, float max) {
		static std::random_device rd;
		static std::mt19937 gen(rd());
		std::uniform_real_distribution<float> dist(min, max);
		return dist(gen);
	}

	Map* map;
	Fish* parentFish = nullptr;
	Ship* ship;
};