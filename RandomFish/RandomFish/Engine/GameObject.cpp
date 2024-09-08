/*
Copyright (C) 2024 DigiPen Institute of Technology
Reproduction or distribution of this file or its contents without
prior written consent is prohibited
File Name:  GameObject.cpp
Project:    CS230 Engine
Author:     Jonathan Holmes, Gyuwon Na
Created:    March 8, 2023
Updated:    May 23, 2024
*/

#include "Engine.h"
#include "GameObject.h"
#include "Collision.h"
#include "ShowCollision.h"

CS230::GameObject::GameObject(Math::vec2 position) : 
    GameObject(position, 0, { 1, 1 })
{}

CS230::GameObject::GameObject(Math::vec2 position, double rotation, Math::vec2 scale) :
    velocity({ 0,0 }),
    position(position),
    scale(scale),
    destroy(false),
    rotation(rotation), 
    current_state(&state_none),
    matrix_outdated(true)
{}

void CS230::GameObject::Draw(Math::TransformationMatrix camera_matrix) {
    Collision* collision = GetGOComponent<Collision>();
    if (collision != nullptr) {
        collision->Draw(camera_matrix);
    }

    
    Sprite* sprite = GetGOComponent<Sprite>();
    if (sprite != nullptr) {
        sprite->Draw(camera_matrix * GetMatrix());
    }
}


const Math::TransformationMatrix& CS230::GameObject::GetMatrix() {
    if (matrix_outdated) {
        object_matrix = Math::TranslationMatrix(position) * Math::RotationMatrix(rotation) * Math::ScaleMatrix(scale);
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
    position = new_position;
}

void CS230::GameObject::UpdatePosition(Math::vec2 delta) {
    position += delta;
}

void CS230::GameObject::SetVelocity(Math::vec2 new_position)
{
    velocity = new_position;
}

void CS230::GameObject::UpdateVelocity(Math::vec2 delta)
{
    velocity += delta;
}

void CS230::GameObject::SetScale(Math::vec2 new_scale)
{
    scale = new_scale;
}

void CS230::GameObject::UpdateScale(Math::vec2 delta)
{
    scale += delta;
}

void CS230::GameObject::SetRotation(double new_rotation)
{
    rotation = new_rotation;
}

void CS230::GameObject::UpdateRotation(double delta)
{
    rotation += delta;
}

void CS230::GameObject::Destroy()
{
    destroy = true;
}

bool CS230::GameObject::Destroyed()
{
    return destroy;
}

void CS230::GameObject::Update(double dt) {
    current_state->Update(this, dt);

    if (velocity.x != 0 || velocity.y != 0) {
        UpdatePosition(velocity * dt);
    }
    current_state->CheckExit(this);
}

void CS230::GameObject::change_state(State* new_state) {
    current_state = new_state;
    current_state->Enter(this);
}

bool CS230::GameObject::IsCollidingWith(GameObject* other_object) {
    Collision* collider = GetGOComponent<Collision>();
    return collider != nullptr && collider->IsCollidingWith(other_object);
}

bool CS230::GameObject::CanCollideWith(GameObjectTypes other_object_type) {
    return false;
}

bool CS230::GameObject::IsCollidingWith(Math::vec2 point) {
    Collision* collider = GetGOComponent<Collision>();
    return collider != nullptr && collider->IsCollidingWith(point);
}
