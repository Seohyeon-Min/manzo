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

class Beat;

class Skillsys;

class Ship : public CS230::GameObject {
public:
    Ship(vec2 start_position);
    GameObjectTypes Type() override { return GameObjectTypes::Ship; }
    std::string TypeName() override { return "Ship:Dal"; }
    void Update(double dt) override;
    void Draw(DrawLayer drawlayer = DrawLayer::Draw) override;
    void Move(double dt);
    void SetDest();
    bool IsShipMoving() { return move; }
    bool CanCollideWith(GameObjectTypes) override;
    void ResolveCollision([[maybe_unused]] GameObject* other_object) override;
    const vec2& GetPosition() const { return GameObject::GetPosition(); }
    //for fuel
    float GetFuel() { return (float)fuel; }
    void FuelUpdate(double dt);
    void SetMaxFuel(double input);
    void HitWithReef(CS230::RectCollision* collision_edge);
    bool IsTouchingReef();
    bool IsFuelZero();

private:
    static constexpr double speed = 7000.f;
    static constexpr float deceleration = 0.80f;
    static constexpr double skidding_speed = 20.f;
    bool moving;
    bool set_dest;
    bool ready_to_move;
    bool move;
    bool clickable = true;
    bool hit_with = false;
    vec2 force = {};
    vec2 destination;
    vec2 direction = { 0,0 };
    Beat* beat;
    Skillsys* skill;

    //for fuel

    bool isCollidingWithReef;
    bool FuelFlag = false;
    double fuel;
    double Maxfuel = 500;
    double baseDecfuel = 2;
    double MoveDecfuel = 0.5;
    double HitDecFuel = 50;
    double fuelcounter = 0;
};