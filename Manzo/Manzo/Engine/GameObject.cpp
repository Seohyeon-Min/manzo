/*
Copyright (C) 2023 DigiPen Institute of Technology
Reproduction or distribution of this file or its contents without
prior written consent is prohibited
File Name:  GameObject.cpp
Project:    CS230 Engine
Author:     Jonathan Holmes, Seohyeon Min
Created:    Aprill 30, 2024
Updated:    04/30/2024
*/

#include "GameObject.h"

CS230::GameObject::GameObject(Math::vec2 position) :
    GameObject(position, 0, { 1, 1 }) {}

CS230::GameObject::GameObject(Math::vec2 position, double rotation, Math::vec2 scale) :
    velocity({ 0,0 }),
    position(position),
    scale(scale),
    rotation(rotation),
    current_state(&state_none),
    destroy(false)
{
    current_state->Enter(this);
}

void CS230::GameObject::Update(double dt) {
    current_state->Update(this, dt);
    if (velocity.x != 0 || velocity.y != 0) {
        UpdatePosition(velocity * dt);
    }
    UpdateGOComponents(dt);
    current_state->CheckExit(this);
}

void CS230::GameObject::change_state(State* new_state) {
    current_state = new_state;
    current_state->Enter(this);
}


void CS230::GameObject::Draw(Math::TransformationMatrix camera_matrix) {
    Sprite* sprite = GetGOComponent<Sprite>();
    if (sprite != nullptr) {
        sprite->Draw(camera_matrix * GetMatrix());
    }
    if (Engine::GetGameStateManager().GetGSComponent<CS230::ShowCollision>() != nullptr && Engine::GetGameStateManager().GetGSComponent<CS230::ShowCollision>()->Enabled()) {
        Collision* collision = GetGOComponent<Collision>();
        if (collision != nullptr) {
            collision->Draw(camera_matrix);
        }
    }
}

bool CS230::GameObject::IsCollidingWith(GameObject* other_object) {
    Collision* collider = GetGOComponent<Collision>();
    return collider != nullptr && collider->IsCollidingWith(other_object);
}

bool CS230::GameObject::IsCollidingWith(Math::vec2 point) {
    Collision* collider = GetGOComponent<Collision>();
    return collider != nullptr && collider->IsCollidingWith(point);
}

bool CS230::GameObject::CanCollideWith([[maybe_unused]] GameObjectTypes other_object_type) {
    return false;
}

const Math::TransformationMatrix& CS230::GameObject::GetMatrix() {
    if (matrix_outdated) {
        object_matrix = Math::TranslationMatrix(position) * Math::RotationMatrix(rotation) * Math::ScaleMatrix(scale);
        matrix_outdated = false;
    }
    return object_matrix;
}

const Math::vec2& CS230::GameObject::GetPosition() const
{
    return position;
}

const Math::vec2& CS230::GameObject::GetVelocity() const
{
    return velocity;
}

const Math::vec2& CS230::GameObject::GetScale() const
{
    return scale;
}

double CS230::GameObject::GetRotation() const
{
    return rotation;
}

void CS230::GameObject::SetPosition(Math::vec2 new_position) {
    matrix_outdated = true;
    position = new_position;
}

void CS230::GameObject::UpdatePosition(Math::vec2 delta) {
    matrix_outdated = true;
    position += delta;
}

void CS230::GameObject::SetVelocity(Math::vec2 new_velocity)
{
    matrix_outdated = true;
    velocity = new_velocity;
}

void CS230::GameObject::UpdateVelocity(Math::vec2 delta)
{
    matrix_outdated = true;
    velocity += delta;
}

void CS230::GameObject::SetScale(Math::vec2 new_scale)
{
    matrix_outdated = true;
    scale = new_scale;
}

void CS230::GameObject::UpdateScale(Math::vec2 delta)
{
    matrix_outdated = true;
    scale += delta;
}

void CS230::GameObject::SetRotation(double new_rotation)
{
    matrix_outdated = true;
    rotation = new_rotation;
}

void CS230::GameObject::UpdateRotation(double delta)
{
    matrix_outdated = true;
    rotation += delta;
}
