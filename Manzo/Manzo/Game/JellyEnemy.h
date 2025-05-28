#pragma once
#include "../Engine/GameObject.h"
#include "../Engine/Procedual.h"
#include "GameObjectTypes.h"
#include "Ship.h"

//heheh

class JellyEnemy : public GameObject
{
public:
	enum class JellyType {
		Up,
		Left,
		Right,
		Down,
		ToPlayer
	};
	JellyEnemy(vec2 start_position, float hight , float lifetime, JellyType jelly_type = JellyType::Up);
	GameObjectTypes Type() override { return GameObjectTypes::JellyEnemy; }
	std::string TypeName() override { return "JellyEnemy"; }
	void Update(double dt) override;
	void Draw(DrawLayer drawlayer = DrawLayer::Draw) override;
	void Move(double dt);
	bool CanCollideWith(GameObjectTypes) override;
	void ResolveCollision([[maybe_unused]] GameObject* other_object) override;

private:
	JellyType jelly_type;
	Ship* ship;

	vec2 position;
	vec2 velocity;
	vec2 targetPosition;

	vec2 static_bullet;
	double lifetime; //same with bpm 
	double speed;
	double speed_for_staticTarget = 500;
	double timeElapsed;
	vec2 toPlayer;
	float hight;
	float distanceToPlayer;
	float waveFrequency = 10.0f;   // 초당 몇 번 흔들릴지
	float waveAmplitude = 100.0f;
	vec2 wave_forward_dir = { 0, 0 };
};