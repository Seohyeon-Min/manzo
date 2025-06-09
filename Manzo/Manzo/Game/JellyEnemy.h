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
	ProceduralChain procedural_jelly;
	JellyType jelly_type;
	Ship* ship;

	std::vector<GLTexture*> texture_vector;
	mat3 test_matrix;
	mat3 test_matrix_1;

	vec2 position;
	vec2 velocity;
	vec2 targetPosition;

	vec2 static_bullet;
	double lifetime; //same with bpm 
	double speed;
	float speed_for_staticTarget = 200;
	double timeElapsed;
	vec2 toPlayer;
	float hight;
	float distanceToPlayer;
	float waveFrequency = 6.0f;   // 초당 몇 번 흔들릴지
	float waveAmplitude = 50.0f;
	vec2 wave_forward_dir = { 0, 0 };
};