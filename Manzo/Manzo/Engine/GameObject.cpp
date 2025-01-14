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

GameObject::GameObject(vec2 position) :
	GameObject(position, 0, { 1, 1 }) {
}

GameObject::GameObject(vec2 position, double rotation, vec2 scale) :
	velocity({ 0,0 }),
	position(position),
	scale(scale),
	rotation(rotation),
	current_state(&state_none),
	destroy(false)
{
	current_state->Enter(this);
}

void GameObject::Update(double dt) {
	current_state->Update(this, dt);
	UpdateGOComponents(dt);
	current_state->CheckExit(this);
}

void GameObject::FixedUpdate(double fixed_dt)
{
	current_state->FixedUpdate(this, fixed_dt);
	if (velocity.x != 0 || velocity.y != 0) {
		UpdatePosition(velocity * (float)fixed_dt);
	}
}

void GameObject::change_state(State* new_state) {
	current_state = new_state;
	current_state->Enter(this);
}


void GameObject::Draw(DrawLayer drawlayer) {
	Sprite* sprite = GetGOComponent<Sprite>();
	if (sprite != nullptr) {
		if (shader == nullptr) {
			shader = Engine::GetShaderManager().GetDefaultShader();
		}

		DrawCall draw_call = {
			sprite,
			&GetMatrix(),
			shader
		};

		if (drawlayer != DrawLayer::Draw) {
			draw_call.sorting_layer = drawlayer;
			Engine::GetRender().AddDrawCall(std::make_unique<DrawCall>(draw_call));
		}
		else {
			Engine::GetRender().AddDrawCall(std::make_unique<DrawCall>(draw_call));  // basic layer
		}
	}
	if (Engine::GetGameStateManager().GetGSComponent<ShowCollision>() != nullptr && Engine::GetGameStateManager().GetGSComponent<ShowCollision>()->Enabled()) {
		Collision* collision = GetGOComponent<Collision>();
		if (collision != nullptr) {
			collision->Draw();
		}
	}
}

void GameObject::Draw(const DrawCall& draw_call)
{
	Sprite* sprite = GetGOComponent<Sprite>();
	if (sprite != nullptr) {
		if (shader == nullptr) {
			if (IsPixelShaderApplicable(Type())) {
				shader = Engine::GetShaderManager().GetShader("pixelate");
			}
			else {
				shader = Engine::GetShaderManager().GetDefaultShader();
			}
		}

		Engine::GetRender().AddDrawCall(std::make_unique<DrawCall>(draw_call));
	}
	if (Engine::GetGameStateManager().GetGSComponent<ShowCollision>() != nullptr && Engine::GetGameStateManager().GetGSComponent<ShowCollision>()->Enabled()) {
		Collision* collision = GetGOComponent<Collision>();
		if (collision != nullptr) {
			collision->Draw();
		}
	}
}

bool GameObject::IsCollidingWith(GameObject* other_object) {
	Collision* collider = GetGOComponent<Collision>();
	return collider != nullptr && collider->IsCollidingWith(other_object);
}

bool GameObject::IsCollidingWith(vec2 point) {
	Collision* collider = GetGOComponent<Collision>();
	return collider != nullptr && collider->IsCollidingWith(point);
}

Math::rect CalculateAABB(const mat3& model_to_world, const vec2& frame_size) {
	vec2 left_bottom_local = { -frame_size.x * 0.5f, -frame_size.y * 0.5f };
	vec2 right_top_local = { frame_size.x * 0.5f, frame_size.y * 0.5f };

	vec3 left_bottom_world = (model_to_world * mat3::build_translation(left_bottom_local)).column2;
	vec3 right_top_world = (model_to_world * mat3::build_translation(right_top_local)).column2;

	//std::cout << "World Transformed Bounds:" << std::endl;
	//std::cout << "  LeftBottom: (" << left_bottom_world.x << ", " << left_bottom_world.y << ")" << std::endl;
	//std::cout << "  RightTop: (" << right_top_world.x << ", " << right_top_world.y << ")" << std::endl;

	return {
		{left_bottom_world.x, left_bottom_world.y},
		{right_top_world.x, right_top_world.y}
	};
}

bool GameObject::IsVisible(const Math::rect& camera_bounds) const
{
	Math::rect object_bounds = cached_aabb;
	return !(object_bounds.Right() <= camera_bounds.Left() ||
		object_bounds.Left() >= camera_bounds.Right() ||
		object_bounds.Top() <= camera_bounds.Bottom() ||
		object_bounds.Bottom() >= camera_bounds.Top());
}

bool GameObject::CanCollideWith([[maybe_unused]] GameObjectTypes other_object_type) {
	return false;
}

const mat3& GameObject::GetMatrix() {
	if (matrix_outdated) {
		object_matrix =
			mat3::build_translation(position) *
			mat3::build_rotation((float)rotation) *
			mat3::build_scale(scale.x, scale.y);

		if(GetGOComponent<Sprite>() != nullptr ) 
		frame_size = (vec2)GetGOComponent<Sprite>()->GetFrameSize();
		else {
			//std::cout << "I don't have a sprite!! : " << TypeName() << std::endl;
		}

		// different with the collisoin one. It is based on the fame size.
		cached_aabb = CalculateAABB(object_matrix, frame_size); 

		matrix_outdated = false;
	}
	return object_matrix;
}

const Math::rect& GameObject::GetAABB() const {
	return cached_aabb;
}

const vec2& GameObject::GetPosition() const
{
	return position;
}

const vec2& GameObject::GetVelocity() const
{
	return velocity;
}

const vec2& GameObject::GetScale() const
{
	return scale;
}

const bool GameObject::GetFlipX() const
{
	return scale.x < 0;
}

double GameObject::GetRotation() const
{
	return rotation;
}

void GameObject::SetPosition(vec2 new_position) {
	position = new_position;
	// why was this commented?
	matrix_outdated = true;
}

void GameObject::UpdatePosition(vec2 delta) {
	position += delta;
	matrix_outdated = true;
}

void GameObject::SetVelocity(vec2 new_velocity)
{
	velocity = new_velocity;
	// why was this commented?
	//matrix_outdated = true;
}

void GameObject::UpdateVelocity(vec2 delta)
{
	velocity += delta;
	matrix_outdated = true;
}

void GameObject::SetScale(vec2 new_scale)
{
	scale = new_scale;
	//matrix_outdated = true;
}

void GameObject::SetFlipX(bool flip) 
{
	if ((flip && scale.x > 0) || (!flip && scale.x < 0))
	{
		scale.x = -scale.x;
	}
}

void GameObject::UpdateScale(vec2 delta)
{
	scale += delta;
	matrix_outdated = true;
}

void GameObject::SetRotation(double new_rotation)
{
	rotation = new_rotation;
	matrix_outdated = true;
}

void GameObject::UpdateRotation(double delta)
{
	rotation += delta;
	matrix_outdated = true;
}