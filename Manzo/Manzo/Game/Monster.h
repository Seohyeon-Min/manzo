#pragma once
#include "../Engine/GameObject.h"

struct MovementRange {
    float radius;
    vec2 pos;
};

class Monster : public GameObject
{
public:
	Monster(Ship* ship, vec2 pos);
	GameObjectTypes Type() override { return GameObjectTypes::Monster; }
	std::string TypeName() override { return "Monster"; }
	void Update(double dt)override;
	void Draw(DrawLayer drawlayer = DrawLayer::Draw)override;
	bool IsPlayerInSight(const vec2& playerPos);
    bool IsPlayerInRange(const vec2& playerPos);
    bool CanCollideWith(GameObjectTypes other_object) override;
    std::array<vec2, 4> GetCollisionBoxPoints();
    void ResolveCollision([[maybe_unused]] GameObject* other_object) override;

private:
	void DrawSight();
    Math::rect GetCollisionBox() { return this->GetGOComponent<RectCollision>()->WorldBoundary_rect(); }
	Ship* ship_ptr;
    Beat* beat;
    float dist_from_ship = 750.f;
	const float sight_radius = 500;
	const float sight_angle = 90;
    vec2 init_pos = { 300,300 };
    vec2 direction;
    vec2 position;
    float speed = 700.f;
    float offset = 7;
    const float initial_offset = 7;
    const double dash_time = 0.86;
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
        float alpha = 0.32f;
        static constexpr float init_alpha = 0.32f;
    };

    class Goback : public State {
    public:
        virtual void Enter(GameObject* object) override;
        virtual void Update(GameObject* object, double dt) override;
        virtual void CheckExit(GameObject* object) override;
        std::string GetName() override { return "State_Move"; }
    };

    Stanby state_stanby;
    Alert state_alert;
    Dash state_go;
    Goback state_goback;
};