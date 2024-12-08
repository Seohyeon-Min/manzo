/*
Copyright (C) 2023 DigiPen Institute of Technology
Reproduction or distribution of this file or its contents without
prior written consent is prohibited
File Name:  Pelican.h
Project:    CS230 Engine
Author:     Seohyeon Min, Won Kim
Created:    June 13, 2024
*/

#pragma once
#include "..\Engine\GameObject.h"
#include "GameObjectTypes.h"
#include "..\Engine\Timer.h"
#include "..\Game\Skill.h"
#include "../Engine/Rect.h"

class Beat;

class Skillsys;

class Ship : public CS230::GameObject {
public:
    Ship(vec2 start_position);
    GameObjectTypes Type() override { return GameObjectTypes::Ship; }
    std::string TypeName() override { return "Ship:Dal"; }
    void Update(double dt) override;
    void FixedUpdate(double fixed_dt) override;
    void Draw(DrawLayer drawlayer = DrawLayer::Draw) override;
    bool IsShipMoving() { return move; }
    bool CanCollideWith(GameObjectTypes) override;
    void ResolveCollision([[maybe_unused]] GameObject* other_object) override;
    const vec2& GetPosition() const { return GameObject::GetPosition(); }
    //for fuel
    float GetFuel() { return (float)fuel; }
    float GetMaxFuel() { return (float)Maxfuel; }
    void FuelUpdate(double dt);
    void SetMaxFuel(double input);
    void HitWithReef(vec2 normal);
    bool IsTouchingReef();
    bool IsFuelZero();
    bool IsShipUnder();

private:
    static constexpr double speed = 6500.f;
    static constexpr float deceleration = 0.88f;
    static constexpr double skidding_speed = 150.f;
    double slow_down_factor;
    bool move;
    bool hit_with = false;
    vec2 force = {};
    vec2 destination;
    vec2 direction = { 0,0 };
    vec2 normal;
    Beat* beat;
    Skillsys* skill;
    Math::rect limit;

    //for fuel

    bool isCollidingWithReef;
    bool FuelFlag = false;
    double fuel;
    double Maxfuel = 1000;
    double baseDecfuel = 2;
    double MoveDecfuel = 0.5;
    double HitDecFuel = 50;
    double fuelcounter = 0;

    //



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
    };

    class State_Hit : public State {
    public:
        virtual void Enter(GameObject* object) override;
        virtual void Update(GameObject* object, double dt) override;
        virtual void FixedUpdate(GameObject* object, double fixed_dt) override;
        virtual void CheckExit(GameObject* object) override;
        std::string GetName() override { return "State_Hit"; }
    };

    State_Idle state_idle;
    State_Move state_move;
    State_Hit state_hit;
};

class Pump : public CS230::GameObject {
public:
    Pump();
    GameObjectTypes Type() override { return GameObjectTypes::Ship; }
    std::string TypeName() override { return "Ship:Dal"; }
    void Update(double dt) override;
    void Draw(DrawLayer drawlayer = DrawLayer::Draw) override;
    float GetRadius() { return radius; }
    void Reset();
    void SetUniforms(const GLShader* shader);

private:
    Beat* beat;
    float max_pump_radius = 100;
    float min_pump_radius = 55;
    float radius = 0;
    float alpha = 0.0;
    bool wait = false;
    bool has_done = false;
};