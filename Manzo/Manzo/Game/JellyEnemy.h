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

	GLTexture* texture_vector;
	mat3 test_matrix;

	vec2 position;
	vec2 velocity;
	vec2 targetPosition;

	vec2 static_bullet;
	double lifetime; //same with bpm 
	double speed;
	double speed_for_staticTarget = 300;
	double timeElapsed;
	vec2 toPlayer;
	float hight;
	float distanceToPlayer;
	float waveFrequency = 5.0f;   // �ʴ� �� �� ��鸱��
	float waveAmplitude = 100.0f;
	vec2 wave_forward_dir = { 0, 0 };
};