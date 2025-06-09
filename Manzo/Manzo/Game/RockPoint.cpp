/*
Copyright (C) 2023 DigiPen Institute of Technology
Reproduction or distribution of this file or its contents without
prior written consent is prohibited
File Name:  RockPoint.cpp
Project:    Manzo
Author:     Won Kim
Created:    November 25, 2024
*/

#include "RockPoint.h"

RockPoint::RockPoint(vec2 position, std::string index) :GameObject(position), index(index)
{
    AddGOComponent(new Sprite("assets/images/rock/rockpoint_temp.spt", this));
}

void RockPoint::Update(double dt)
{
    GameObject::Update(dt);
}

void RockPoint::Draw()
{
    GameObject::Draw();
}

bool RockPoint::CanCollideWith(GameObjectTypes other_object)
{
    switch (other_object) {
    case GameObjectTypes::MovingRock:
        return true;
        break;
    }
    
    return false;
}

void RockPoint::ResolveCollision(GameObject* other_object)
{
    if (other_object->Type() == GameObjectTypes::MovingRock) {
        MovingRock* moving_rock = static_cast<MovingRock*>(other_object);
        if (this->GetIndex() == moving_rock->GetOriginalPoly().polyindex) {
            auto* collision_edge = this->GetGOComponent<MAP_SATCollision>();
            if (collision_edge == nullptr) {
                // maybe an error?
            }
            for (auto& rock : moving_rock->GetRockGroup()->GetMovingRocks()) {
                rock->Hit(true);    // rock changes state to State_Pop

            }
        }
    }
}
