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

class Ship : public CS230::GameObject {
public:
    Ship(Math::vec2 start_position, GameObject* standing_on);
    GameObjectTypes Type() override { return GameObjectTypes::Ship; }
    std::string TypeName() override { return "Pelican"; }
    void Update(double dt) override;
    void Draw(Math::TransformationMatrix camera_matrix) override;
    bool CanCollideWith(GameObjectTypes) override;
    void ResolveCollision([[maybe_unused]] GameObject* other_object) override;
    const Math::vec2& GetPosition() const { return GameObject::GetPosition(); }

private:
    static constexpr double jump_velocity = 700; //650
    double velocity = 300; //650
    bool moving;
};
