/*
Copyright (C) 2023 DigiPen Institute of Technology
Reproduction or distribution of this file or its contents without
prior written consent is prohibited
File Name:  Rock.cpp
Project:    Manzo
Author:     Won Kim
Created:    November 25, 2024
*/

#include "Rock.h"

Rock::Rock(Polygon poly) :GameObject({ 0,0 }), poly(poly)
{
    //this->SetCenter();  //set rock positions
}

void Rock::Update(double dt)
{
    GameObject::Update(dt);
}

void Rock::Draw()
{
    GameObject::Draw();
}


bool Rock::CanCollideWith(GameObjectTypes other_object)
{
    switch (other_object) {
    case GameObjectTypes::RockPoint:
        return true;
        break;
    }

    return false;
}

void Rock::ResolveCollision(GameObject* other_object)
{
    if (other_object->Type() == GameObjectTypes::RockPoint) {
        auto* collision_edge = this->GetGOComponent<CS230::MAP_SATCollision>();
        if (collision_edge == nullptr) {
            // maybe an error?
        }

        RockGroup* rockgroup = this->GetRockGroup();
        for (auto& rock : rockgroup->GetRocks()) {
            rock->PopBack({ -1, 0 }, 10);
        }

    }
}

void Rock::SetCenter() {
    vec2 center = { 0, 0 };
    std::vector<vec2> vertices = this->GetPolygon().vertices;
    for (vec2 vertice : vertices) {
        center.x += vertice.x;
        center.y += vertice.y;
    }
    center.x /= vertices.size();
    center.y /= vertices.size();
    SetPosition(center);
}
vec2 Rock::Normalize(const vec2& vec) {
    float length = std::sqrt(vec.x * vec.x + vec.y * vec.y);
    vec2 normalized_vec = {0.f, 0.f};

    if (length == 0) { return vec; }

    normalized_vec.x = vec.x / length;
    normalized_vec.y = vec.y / length;

    return normalized_vec;
}
void Rock::Pop(const vec2& direction, float length) {
    float speed = 1000;
    vec2 velo = { Normalize(direction).x * speed, Normalize(direction).y * speed };
    SetVelocity(velo);

    //SetVelocity(-velo); // return first location
}

void Rock::PopBack(const vec2& direction, float length) {
    float speed = 1000;
    vec2 velo = { -Normalize(direction).x * speed, -Normalize(direction).y * speed };
    SetVelocity(velo);

    //SetVelocity(-velo); // return first location
}

