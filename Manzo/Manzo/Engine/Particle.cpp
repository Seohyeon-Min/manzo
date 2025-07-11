/*
Copyright (C) 2023 DigiPen Institute of Technology
Reproduction or distribution of this file or its contents without
prior written consent is prohibited
File Name:  Particle.cpp
Project:    CS230 Engine
Author:     Min Seohyeon
Created:    June 8, 2024
*/

#include "Particle.h"

Particle::Particle(const std::filesystem::path& sprite_file):
	GameObject({0,0})
{
	AddGOComponent(new Sprite(sprite_file, this));
}

void Particle::Start(vec2 position, vec2 velocity, double _max_life, vec2 scale)
{
	SetPosition(position);
	SetVelocity(velocity);
	SetScale(scale);
	life = _max_life - life_offset;
	max_life = _max_life - life_offset;
	GetGOComponent<Sprite>()->Reset();
}

void Particle::Update(double dt)
{
	if (!Alive()) return;
	life -= dt;
	if (Alive())
		GameObject::Update(dt);

	switch (move_type)
	{
	case MoveType::LINEAR:
		break;
	case MoveType::CURVE:
		break;
	case MoveType::RANDOM:
		break;
	case MoveType::TOTHEPLAYER:
		break;
	default:
		break;
	}
}



void Particle::Draw(DrawLayer dl)
{
	if(Alive())
		if (shader == nullptr) {
			Sprite* sprite = GetGOComponent<Sprite>();

			DrawCall draw_call = {
				sprite,                       // Texture to draw
				&GetMatrix(),                          // Transformation matrix
				Engine::GetShaderManager().GetDefaultShader(), // Shader to use
			};

			draw_call.settings.do_blending = true;
			draw_call.sorting_layer = drawlayer;

			GameObject::Draw(draw_call);
		}
		else {

			Sprite* sprite = GetGOComponent<Sprite>();

			DrawCall draw_call = {
				sprite,                       // Texture to draw
				&GetMatrix(),                          // Transformation matrix
				shader, // Shader to use
			};

			draw_call.settings.do_blending = true;
			draw_call.SetUniforms = [this](const GLShader* shader) { SetAlpha(shader);};
			draw_call.sorting_layer = drawlayer;

			GameObject::Draw(draw_call);
		}
}

void Particle::SetAlpha(const GLShader* shader)
{
	float alpha = float(life / max_life);
	shader->SendUniform("uAlpha", alpha);
}
