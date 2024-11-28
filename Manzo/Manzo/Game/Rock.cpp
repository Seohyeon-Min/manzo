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

Rock::Rock(Polygon poly) :GameObject({ 0,0 }), poly(poly), hit(false)
{
    SetCenter();

    if (Engine::GetGameStateManager().GetStateName() == "Mode1") {
        current_state = &state_idle;
        current_state->Enter(this);
    }
}

void Rock::Update(double dt)
{
    GameObject::Update(dt);
}

void Rock::Draw()
{
    GameObject::Draw();
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
    
}
vec2 Rock::Normalize(const vec2& vec) {
    float length = std::sqrt(vec.x * vec.x + vec.y * vec.y);
    vec2 normalized_vec = {0.f, 0.f};

    if (length == 0) { return vec; }

    normalized_vec.x = vec.x / length;
    normalized_vec.y = vec.y / length;

    return normalized_vec;
}
void Rock::Pop(const vec2& direction, float speed) {
    vec2 normVec = Normalize(direction);
    vec2 velo = { normVec.x * speed, normVec.y * speed };
    SetVelocity(velo);
    this->GetRockGroup()->SetVelocity(velo);
}

void Rock::PopBack(const vec2& direction, float speed) {
    vec2 velo = { Normalize(direction).x * speed, Normalize(direction).y * speed };
    SetVelocity(velo);
    this->GetRockGroup()->SetVelocity(velo);

}

bool Rock::IsRange(const vec2& current_position) {
    vec2 distanceVec = current_position - vec2({0, 0});
    float distance = distanceVec.x * distanceVec.x + distanceVec.y * distanceVec.y;
    return distance <= range;
}


//==========================State==================================

void Rock::State_Idle::Enter(GameObject* object) {
    Rock* rock = static_cast<Rock*>(object);
    rock->Hit(false);
}
void Rock::State_Idle::Update([[maybe_unused]] GameObject* object, [[maybe_unused]] double dt) {}
void Rock::State_Idle::CheckExit(GameObject* object) {
    Rock* rock = static_cast<Rock*>(object);
    rock->SetVelocity({ 0, 0 });
    rock->GetRockGroup()->SetVelocity({0, 0});
    if (rock->hit) {
        rock->change_state(&rock->state_pop);
    }
}

void Rock::State_Pop::Enter(GameObject* object) {
    Rock* rock = static_cast<Rock*>(object);
    rock->Hit(false);
}
void Rock::State_Pop::Update([[maybe_unused]] GameObject* object, [[maybe_unused]] double dt) {
    Rock* rock = static_cast<Rock*>(object);
    RockGroup* rockgroup = rock->GetRockGroup();
    vec2 direction = { rockgroup->GetRockPoint()->GetPosition() - rockgroup->GetPosition()};
    rock->Pop(direction, rock->pop_speed);
}
void Rock::State_Pop::CheckExit(GameObject* object) {
    Rock* rock = static_cast<Rock*>(object);
    if (rock->hit) {
        rock->change_state(&rock->state_popback);
    }
}

void Rock::State_PopBack::Enter(GameObject* object) {
    Rock* rock = static_cast<Rock*>(object);
    rock->Hit(false);
}
void Rock::State_PopBack::Update([[maybe_unused]] GameObject* object, [[maybe_unused]] double dt) {
    std::cout << "popping" << "\n";
    Rock* rock = static_cast<Rock*>(object);
    RockGroup* rockgroup = rock->GetRockGroup();

    vec2 back_position = rockgroup->GetRockPoint()->GetPosition();
    vec2 point_position = rockgroup->GetPosition();

    for (auto& rock : rockgroup->GetRocks()) {
        vec2 direction = point_position - back_position;
        rock->PopBack(direction, rock->pop_back_speed);
    }
}

void Rock::State_PopBack::CheckExit(GameObject* object) {
    Rock* rock = static_cast<Rock*>(object);
    if (rock->IsRange(rock->GetPosition())) {
        rock->change_state(&rock->state_idle);
    }
}
