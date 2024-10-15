/*
Copyright (C) 2023 DigiPen Institute of Technology
Reproduction or distribution of this file or its contents without
prior written consent is prohibited
File Name:  Camera.cpp
Project:    CS230 Engine
Author:     Seohyeon Min
Created:    March 22, 2023
*/

#include "Camera.h"

CS230::Camera::Camera(Math::rect player_zone) : player_zone(player_zone), position({ 0,0 })
{

}

void CS230::Camera::SetPosition(vec2 new_position)
{
    position = new_position;
}

const vec2& CS230::Camera::GetPosition() const
{
    return position;
}

void CS230::Camera::SetLimit(Math::irect new_limit)
{
    limit = new_limit;
}

mat3 CS230::Camera::GetMatrix() {
    return mat3(-position);
}


void CS230::Camera::Update(const vec2& player_position) {

    float lerpFactor = 0.1f; // (0.0 ~ 1.0)

    vec2 target_position = position;

    if (player_position.x > player_zone.Right() + position.x) {
        target_position.x = player_position.x - player_zone.Right();
    }
    if (player_position.x - position.x < player_zone.Left()) {
        target_position.x = player_position.x - player_zone.Left();
    }

    position.x += (target_position.x - position.x) * lerpFactor;

    if (player_position.y > player_zone.Top() + position.y) {
        target_position.y = player_position.y - player_zone.Top();
    }
    if (player_position.y - position.y < player_zone.Bottom()) {
        target_position.y = player_position.y - player_zone.Bottom();
    }

    position.y += (target_position.y - position.y) * lerpFactor;


    //if (position.x < limit.left()) {
    //    position.x = limit.left();
    //}
    //if (position.x > limit.right()) {
    //    position.x = limit.right();
    //}
    //if (position.y < limit.bottom()) {
    //    position.y = limit.bottom();
    //}
    //if (position.y > limit.top()) {
    //    position.y = limit.top();
    //}
}

