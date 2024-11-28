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
    AddGOComponent(new CS230::Sprite("assets/images/rockpoint_temp.spt", this));
}

void RockPoint::Update(double dt)
{
    CS230::GameObject::Update(dt);
}

void RockPoint::Draw()
{
    CS230::GameObject::Draw();
}

bool RockPoint::CanCollideWith(GameObjectTypes other_object)
{
    switch (other_object) {
    case GameObjectTypes::Rock:
        return true;
        break;
    }
    return false;
}

void RockPoint::ResolveCollision(GameObject* other_object)
{
    if (other_object->Type() == GameObjectTypes::Rock) {
        Rock* rock = static_cast<Rock*>(other_object);
        RockGroup* rockgroup = rock->GetRockGroup();
        if (this->GetIndex() == rock->GetPolygon().polyindex) {
            auto* collision_edge = this->GetGOComponent<CS230::MAP_SATCollision>();
            if (collision_edge == nullptr) {
                // maybe an error?
            }

            vec2 back_position = this->GetPosition();
            vec2 point_position = rockgroup->GetPosition();

            for (auto& rock : rockgroup->GetRocks()) {
                vec2 direction = point_position - back_position;
                float speed = 200;
                rock->PopBack(direction, speed);
            }
        }
    }
}
