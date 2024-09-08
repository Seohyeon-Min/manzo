#pragma once
#include "../Engine/GameObject.h"
#include "GameObjectTypes.h"

class Fish : public CS230::GameObject
{
public:
	Fish() : Fish(nullptr) { };
	Fish(Fish* parent);
	static constexpr double default_velocity = 10.0;
	GameObjectTypes Type() override { return GameObjectTypes::Fish; }
	std::string TypeName() override { return "Fish"; }

	static constexpr double default_scales[] = { 1.0, 1.5, 2.0 };

private:
	void Update(double dt);

	Math::vec2 start_position;
	int swimming_range = 15;
	int size;
};