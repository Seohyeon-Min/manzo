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
	bool CanCollideWith(GameObjectTypes) override;
	void ResolveCollision([[maybe_unused]] GameObject* other_object) override;

	static constexpr double default_scales[] = { 1.0, 1.5, 2.0 };
	vec2 start_position;

	void SetAvoidanceTimer(double time) { avoidanceTimer = time; }
	double GetAvoidanceTimer() const { return avoidanceTimer; }
	void UpdateAvoidanceTimer(double deltaTime) {
		if (avoidanceTimer > 0) avoidanceTimer -= deltaTime;
	}

private:
	void Update(double dt);
	void Draw();
	//void ResolveCollision(GameObject* other_object) override;
	//bool CanCollideWith(GameObjectTypes obj) override;

	int size;
	double swimming_range = 15.0;
	double avoidanceTimer = 0.0;
};