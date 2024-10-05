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
	Math::vec2 start_position = { 0,0 };

	enum FishType
	{
		Fish1, Fish2, Fish3
	};
	int type = FishType::Fish1;

	bool collided = false;

private:
	void Update(double dt);
	void Draw(Math::TransformationMatrix camera_matrix);

	int size = 0;
	double swimming_range = 15.0;
};
