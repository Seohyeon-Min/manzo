#pragma once
#include "../Engine/GameObject.h"

struct MovementRange {
    float radius;
    vec2 pos;
};

struct Vision {
    float radius = 500.f;
    float angle = 90.f;
    float dist_from_ship = 750.f;
};

struct Movement {
    vec2 init_pos = { 300, 300 };
    vec2 position;
    vec2 direction;
    float speed = 700.f;
    MovementRange range;
};

struct Dash {
    float offset = 7.f;
    const float initial_offset = 7.f;
    const double dash_time = 0.86;
    RealTimeTimer* timer = nullptr;
};

struct TransformData {
    float scale = 1.f;
    const float max_scale = 2.f;
    float angle = 0.f;
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
    void SetUni(const GLShader* shader);
	void DrawSight();
    Math::rect GetCollisionBox() { return this->GetGOComponent<RectCollision>()->WorldBoundary_rect(); }
	Ship* ship_ptr;
    Beat* beat;
    bool  wait = false;

    Vision vision;
    Movement movement;
    Dash dash;
    TransformData transform;

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