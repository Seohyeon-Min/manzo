#pragma once
#include "../Engine/GameObject.h"
#include "GameObjectTypes.h"
//heheh

class BossBullet : public GameObject
{
public:
	BossBullet(vec2 Boss_position, float lifetime);
	GameObjectTypes Type() override { return GameObjectTypes::BossBullet; }
	std::string TypeName() override { return "BossBullet"; }
	void Update(double dt) override;
	void Draw(DrawLayer drawlayer = DrawLayer::Draw) override;
	void Move(double dt);
	bool CanCollideWith(GameObjectTypes) override;
	void ResolveCollision([[maybe_unused]] GameObject* other_object) override;

private:

	vec2 position;
	vec2 velocity;
	vec2 targetPosition;
	double lifetime; //same with bpm 
	double speed;
	double timeElapsed;
	vec2 toPlayer;
	float distanceToPlayer;
};