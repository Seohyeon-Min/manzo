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

class Beat;

class Ship : public CS230::GameObject {
public:
    Ship(vec2 start_position);
    GameObjectTypes Type() override { return GameObjectTypes::Ship; }
    std::string TypeName() override { return "Ship:Dal"; }
    void Update(double dt) override;
    void Draw() override;
    void Move(double dt);
    void SetDest();
    bool CanCollideWith(GameObjectTypes) override;
    void ResolveCollision([[maybe_unused]] GameObject* other_object) override;
    const vec2& GetPosition() const { return GameObject::GetPosition(); }

private:
    static constexpr double initialSpeed = 1000.f;
    static constexpr float deceleration = 50.0f;
    float currentSpeed = initialSpeed;
    bool moving;
    bool set_dest;
    bool ready_to_move;
    bool move;
    vec2 destination;
    vec2 initialPosition;
    Beat* beat;
};
