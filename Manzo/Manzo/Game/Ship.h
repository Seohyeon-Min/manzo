#pragma once
#include "..\Engine\GameObject.h"
#include "GameObjectTypes.h"
#include "..\Engine\Timer.h"
#include "..\Game\Skill.h"
#include "../Engine/Rect.h"
#include "Rock.h"
#include "BounceBehavior.h"

class Beat;
class Skillsys;

class Ship : public GameObject {
public:
    Ship(vec2 start_position);
    GameObjectTypes Type() override { return GameObjectTypes::Ship; }
    std::string TypeName() override { return "Ship:Dal"; }
    void Update(double dt) override;
    void FixedUpdate(double fixed_dt) override;
    void Draw(DrawLayer drawlayer = DrawLayer::Draw) override;
    bool IsShipMoving() { return move; }
    bool CanCollideWith(GameObjectTypes) override;
    void ResolveCollision(GameObject* other_object) override;

    // fuel
    void SetMaxFuel(float input);
    float GetFuel() { return (float)fuel; }
    float GetMaxFuel() { return (float)Maxfuel; }
    void FuelUpdate(double dt);
    bool IsTouchingReef();
    bool IsFuelZero();
    bool IsShipUnder();
    void DeclineFuel(float d) { fuel -= d; }

    const vec2 GetDashPos() { return dash_target; }

private:
    void HitWithBounce(GameObject* other_object, vec2 initial_velocity);
    void ReduceFuel(float value);

    static constexpr double speed = 6500.f;
    static constexpr float deceleration = 0.88f;
    static constexpr double skidding_speed = 150.f;
    double slow_down_factor = 0.02;
    float toi = 0;
    bool move;
    bool hit_with = false;
    bool should_resolve_collision = false;
    float slow_down = 0;
    vec2 force = {};
    vec2 destination;
    vec2 direction = { 0,0 };
    vec2 normal;
    vec2 collisionPos;
    Beat* beat;
    Skillsys* skill;
    Math::rect limit;
    const double fuel_bubble_time = 0.03;
    const double collide_time = 1.1;
    const float camera_shake = 10;
    const double invincibility_time = 1.6;
    Timer* fuel_bubble_timer;
    Timer* invincibility_timer;
    RealTimeTimer* collide_timer;
    bool isCollidingWithReef;
    bool FuelFlag = false;
    bool can_dash = true;
    float fuel;
    float Maxfuel = 1000.f;


    float baseDecfuel = 0.8f;
    float MoveDecfuel = 0.1f;
    float RockHitDecFuel = 50.f;
    float MonsHitDecFuel = 80.f;
    float BossBulletHitDecFuel = 150.f;


    float fuelcounter = 0.f;
    Rock* before_nearest_rock = nullptr;
    Rock* nearestRock = nullptr;
    GLTexture* hit_text;
    //

    vec2 dash_target;

    BounceBehavior* bounceBehavior;

    class State_Idle : public State {
    public:
        virtual void Enter(GameObject* object) override;
        virtual void Update(GameObject* object, double dt) override;
        virtual void CheckExit(GameObject* object) override;
        std::string GetName() override { return "State_Idle"; }
    };

    class State_Move : public State {
    public:
        virtual void Enter(GameObject* object) override;
        virtual void Update(GameObject* object, double dt) override;
        virtual void FixedUpdate(GameObject* object, double fixed_dt) override;
        virtual void CheckExit(GameObject* object) override;
        std::string GetName() override { return "State_Move"; }
        bool skip_enter = false;
    };

    class State_Die : public State {
    public:
        virtual void Enter(GameObject* object) override;
        virtual void Update(GameObject* object, double dt) override;
        virtual void CheckExit(GameObject* object) override;
        std::string GetName() override { return "State_Die"; }
    };

    State_Idle state_idle;
    State_Move state_move;
    State_Die state_die;
};

class Pump : public GameObject {
public:
    Pump();
    GameObjectTypes Type() override { return GameObjectTypes::Ship; }
    std::string TypeName() override { return "Ship:Dal_Pump"; }
    void Update(double dt) override;
    void Draw(DrawLayer drawlayer = DrawLayer::Draw) override;
    float GetRadius() { return radius; }
    void Reset();
    void SetUniforms(const GLShader* shader);

private:
    Beat* beat;
    float max_pump_radius = 50;
    float min_pump_radius = 27.8f;
    float radius = 0;
    float alpha = 0.0;
    bool wait = false;
    bool has_done = false;
};