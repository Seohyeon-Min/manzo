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
	life = _max_life;
	max_life = _max_life;
	GetGOComponent<Sprite>()->Reset();
}

void Particle::Update(double dt)
{
	life -= dt;
	if (Alive())
		GameObject::Update(dt);
}



void Particle::Draw(DrawLayer dl)
{
	if(Alive())
		if(shader == nullptr) GameObject::Draw(drawlayer);
		else {
			DrawSettings settings;
			settings.do_blending = true;
			Sprite* sprite = GetGOComponent<Sprite>();

			DrawCall draw_call = {
				sprite,                       // Texture to draw
				&GetMatrix(),                          // Transformation matrix
				Engine::GetShaderManager().GetShader("change_alpha"), // Shader to use
				[this](const GLShader* shader) {
					SetAlpha(shader);
				},
				settings
			};

			GameObject::Draw(draw_call, drawlayer);
		}
}

void Particle::SetAlpha(const GLShader* shader)
{
	float alpha = float(life / max_life);
	shader->SendUniform("uAlpha", alpha);
}
