#pragma once
#include "../Engine/GameObject.h"
#include "../Engine/Procedual.h"
#include "GameObjectTypes.h"
#include "Ship.h"
//heheh

class BossBullet : public GameObject
{
public:
	enum class BulletType {
		Homing,          // 플레이어 추적
		StaticTarget,    // 생성 시 위치 고정
		Wave,            // 파도처럼 움직임
		Accelerating     // 점점 빨라짐
	};
	BossBullet(vec2 Boss_position, float lifetime, BulletType type = BulletType::Homing);
	GameObjectTypes Type() override { return GameObjectTypes::BossBullet; }
	std::string TypeName() override { return "BossBullet"; }
	void Update(double dt) override;
	void Draw(DrawLayer drawlayer = DrawLayer::Draw) override;
	void Move(double dt);
	bool CanCollideWith(GameObjectTypes) override;
	void ResolveCollision([[maybe_unused]] GameObject* other_object) override;

private:
	BulletType bulletType;
	ProceduralChain procedual;
	Ship* ship;

	Timer* particle_timer;
	const double particle_time = 0.03;

	vec2 position;
	vec2 velocity;
	vec2 targetPosition;	

	vec2 static_bullet;

	double lifetime; //same with bpm 
	double speed;
	double speed_for_staticTarget = 500;
	double timeElapsed;
	vec2 toPlayer;
	float distanceToPlayer;

	float waveFrequency = 10.0f;   // 초당 몇 번 흔들릴지
	float waveAmplitude = 100.0f;
	vec2 wave_forward_dir = { 0, 0 };
};