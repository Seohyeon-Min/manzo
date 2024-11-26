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

#include <optional> 

CS230::GameObject::GameObject(vec2 position) :
	GameObject(position, 0, { 1, 1 }) {
}

CS230::GameObject::GameObject(vec2 position, double rotation, vec2 scale) :
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
	UpdateGOComponents(dt);
	current_state->CheckExit(this);
}

void CS230::GameObject::FixedUpdate(double fixed_dt)
{
	current_state->FixedUpdate(this, fixed_dt);
	if (velocity.x != 0 || velocity.y != 0) {
		UpdatePosition(velocity * (float)fixed_dt);
	}
}

void CS230::GameObject::change_state(State* new_state) {
	current_state = new_state;
	current_state->Enter(this);
}


void CS230::GameObject::Draw(DrawLayer drawlayer) {
	Sprite* sprite = GetGOComponent<Sprite>();
	if (sprite != nullptr) {
		if (shader == nullptr) {
			//shader = Engine::GetShaderManager().GetDefaultShader();
			if (IsPixelShaderApplicable(Type())) {
				// 픽셀화 셰이더 적용
				shader = Engine::GetShaderManager().GetShader("pixelate");
				shader->SendUniform("uPixelSize", 0.05f);
				//SetGlobalShader(pixelShader);
			}
			else {
				// 기본 셰이더 적용
				shader = Engine::GetShaderManager().GetDefaultShader();
				//SetGlobalShader(defaultShader);
			}
		}

		DrawCall draw_call = {
			sprite->GetTexture(),
			&GetMatrix(),
			shader
		};

		// DrawLayer가 기본값이 아니면 `AddDrawCall`에 추가
		if (drawlayer != DrawLayer::Draw) {
			Engine::GetRender().AddDrawCall(draw_call, drawlayer);
		}
		else {
			Engine::GetRender().AddDrawCall(draw_call);  // basic layer
		}
	}
	if (Engine::GetGameStateManager().GetGSComponent<CS230::ShowCollision>() != nullptr && Engine::GetGameStateManager().GetGSComponent<CS230::ShowCollision>()->Enabled()) {
		Collision* collision = GetGOComponent<Collision>();
		if (collision != nullptr) {
			collision->Draw();
		}
	}
}

bool CS230::GameObject::IsCollidingWith(GameObject* other_object) {
	Collision* collider = GetGOComponent<Collision>();
	return collider != nullptr && collider->IsCollidingWith(other_object);
}

bool CS230::GameObject::IsCollidingWith(vec2 point) {
	Collision* collider = GetGOComponent<Collision>();
	return collider != nullptr && collider->IsCollidingWith(point);
}

bool CS230::GameObject::CanCollideWith([[maybe_unused]] GameObjectTypes other_object_type) {
	return false;
}

const mat3& CS230::GameObject::GetMatrix() {
	if (matrix_outdated) {
		object_matrix =
			mat3::build_translation(position) *
			mat3::build_rotation((float)rotation) *
			mat3::build_scale(scale.x, scale.y);
		matrix_outdated = false;
	}
	return object_matrix;
}

const vec2& CS230::GameObject::GetPosition() const
{
	return position;
}

const vec2& CS230::GameObject::GetVelocity() const
{
	return velocity;
}

const vec2& CS230::GameObject::GetScale() const
{
	return scale;
}

double CS230::GameObject::GetRotation() const
{
	return rotation;
}

void CS230::GameObject::SetPosition(vec2 new_position) {
	position = new_position;
	//matrix_outdated = true;
}

void CS230::GameObject::UpdatePosition(vec2 delta) {
	position += delta;
	matrix_outdated = true;
}

void CS230::GameObject::SetVelocity(vec2 new_velocity)
{
	velocity = new_velocity;
	//matrix_outdated = true;
}

void CS230::GameObject::UpdateVelocity(vec2 delta)
{
	velocity += delta;
	matrix_outdated = true;
}

void CS230::GameObject::SetScale(vec2 new_scale)
{
	scale = new_scale;
	//matrix_outdated = true;
}

void CS230::GameObject::UpdateScale(vec2 delta)
{
	scale += delta;
	matrix_outdated = true;
}

void CS230::GameObject::SetRotation(double new_rotation)
{
	rotation = new_rotation;
	matrix_outdated = true;
}

void CS230::GameObject::UpdateRotation(double delta)
{
	rotation += delta;
	matrix_outdated = true;
}