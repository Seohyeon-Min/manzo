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

Rock::Rock(Polygon original_poly, Polygon modified_poly, vec2 position, double rotation, vec2 scale) 
    :GameObject(position, rotation, scale), original_poly(original_poly), modified_poly(modified_poly)
{
    
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
    std::vector<vec2> vertices = this->GetOriginalPoly().vertices;
    for (vec2 vertice : vertices) {
        center.x += vertice.x;
        center.y += vertice.y;
    }
    center.x /= vertices.size();
    center.y /= vertices.size();

}

// Obstacle Rock
ObstacleRock::ObstacleRock(Polygon original_poly, Polygon modified_poly, vec2 position, double rotation, vec2 scale)
    :Rock( original_poly, modified_poly, position, rotation, scale)
{}

// Moving Rock


MovingRock::MovingRock(Polygon original_poly, Polygon modified_poly, vec2 position, double rotation, vec2 scale) 
    :Rock(original_poly, modified_poly, position, rotation, scale), hit(false)
{
    SetCenter();

    if (Engine::GetGameStateManager().GetStateName() == "Mode1") {
        current_state = &state_idle;
        current_state->Enter(this);
    }
}
void MovingRock::Pop(const vec2& direction, float speed) {
    vec2 normVec = direction.Normalize();
    vec2 velo = { normVec.x * speed, normVec.y * speed };
    SetVelocity(velo);
    this->GetRockGroup()->SetVelocity(velo);
}

void MovingRock::PopBack(const vec2& direction, float speed) {
    vec2 normVec = direction.Normalize();
    vec2 velo = { normVec.x * speed, normVec.y * speed };
    SetVelocity(velo);
    this->GetRockGroup()->SetVelocity(velo);

}

bool MovingRock::IsRange(const vec2& current_position) {
    vec2 distanceVec = current_position - vec2({0, 0});
    float distance = distanceVec.x * distanceVec.x + distanceVec.y * distanceVec.y;
    return distance <= range;
}


//==========================State==================================


void MovingRock::State_Idle::Enter(GameObject* object) {
    MovingRock* rock = static_cast<MovingRock*>(object);
    rock->Hit(false);
}
void MovingRock::State_Idle::Update([[maybe_unused]] GameObject* object, [[maybe_unused]] double dt) {}
void MovingRock::State_Idle::CheckExit(GameObject* object) {
    MovingRock* rock = static_cast<MovingRock*>(object);
    rock->SetVelocity({ 0, 0 });
    rock->GetRockGroup()->SetVelocity({0, 0});
    if (rock->hit) {
        rock->change_state(&rock->state_pop);
    }
}

void MovingRock::State_Pop::Enter(GameObject* object) {
    MovingRock* rock = static_cast<MovingRock*>(object);
    rock->Hit(false);
}
void MovingRock::State_Pop::Update([[maybe_unused]] GameObject* object, [[maybe_unused]] double dt) {
    MovingRock* rock = static_cast<MovingRock*>(object);
    RockGroup* rockgroup = rock->GetRockGroup();
    //vec2 direction = { rockgroup->GetRockPoint()->GetPosition() - rockgroup->GetPosition()};
    //rock->Pop(direction, rock->pop_speed);
}
void MovingRock::State_Pop::CheckExit(GameObject* object) {
    MovingRock* rock = static_cast<MovingRock*>(object);
    if (rock->hit) {
        rock->change_state(&rock->state_popback);
    }
}

void MovingRock::State_PopBack::Enter(GameObject* object) {
    MovingRock* rock = static_cast<MovingRock*>(object);
    rock->Hit(false);
}
void MovingRock::State_PopBack::Update([[maybe_unused]] GameObject* object, [[maybe_unused]] double dt) {
    //std::cout << "popping" << "\n";
    MovingRock* rock = static_cast<MovingRock*>(object);
    RockGroup* rockgroup = rock->GetRockGroup();

    //vec2 back_position = rockgroup->GetRockPoint()->GetPosition();
    vec2 point_position = rockgroup->GetPosition();

    for (auto& rock : rockgroup->GetMovingRocks()) {
        vec2 direction = { 0, 0 };
        //direction = point_position - back_position;
        rock->PopBack(direction, rock->pop_back_speed);
    }
}

void MovingRock::State_PopBack::CheckExit(GameObject* object) {
    MovingRock* rock = static_cast<MovingRock*>(object);
    if (rock->IsRange(rock->GetPosition())) {
        rock->change_state(&rock->state_idle);
    }
}

