#pragma once
#include "../Engine/GameObject.h"

struct MovementRange {
    float radius;
    vec2 pos;
};

class Monster : public GameObject
{
public:
	Monster(Ship* ship);
	GameObjectTypes Type() override { return GameObjectTypes::Monster; }
	std::string TypeName() override { return "Monster"; }
	void Update(double dt)override;
	void Draw(DrawLayer drawlayer = DrawLayer::Draw)override;
	bool IsPlayerInSight(const vec2& playerPos);
    bool IsPlayerInRange(const vec2& playerPos);

	void DrawSight();

private:
	Ship* ship_ptr;
    Beat* beat;
	const float sight_radius = 400;
	const float sight_angle = 90;
    constexpr static vec2 init_pos = { 300,300 };
    vec2 direction;
    vec2 position;
    float speed = 700.f;
    float offset = 7;
    const float initial_offset = 7;
    const double dash_time = 0.7;
    RealTimeTimer* dash_timer;
    MovementRange movement_range;
    bool wait = false;
    float scale = 1.0f;
    const float max_scale = 2.0f;

    class Stanby : public State {
    public:
        virtual void Enter(GameObject* object) override;
        virtual void Update(GameObject* object, double dt) override;
        virtual void CheckExit(GameObject* object) override;
        std::string GetName() override { return "State_Idle"; }
    };

    class Alert : public State {
    public:
        virtual void Enter(GameObject* object) override;
        virtual void Update(GameObject* object, double dt) override;
        virtual void CheckExit(GameObject* object) override;
        std::string GetName() override { return "State_Move"; }
    };

    class Dash : public State {
    public:
        virtual void Enter(GameObject* object) override;
        virtual void Update(GameObject* object, double dt) override;
        virtual void CheckExit(GameObject* object) override;
        std::string GetName() override { return "State_Move"; }
    };

    Stanby state_stanby;
    Alert state_alert;
    Dash state_go;
};