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

CS230::Particle::Particle(const std::filesystem::path& sprite_file):
	GameObject({0,0})
{
	AddGOComponent(new CS230::Sprite(sprite_file, this));
}

void CS230::Particle::Start(vec2 position, vec2 velocity, double _max_life, vec2 scale)
{
	SetPosition(position);
	SetVelocity(velocity);
	SetScale(scale);
	life = _max_life;
	max_life = _max_life;
	//GetGOComponent<CS230::Sprite>()->Reset();
}

void CS230::Particle::Update(double dt)
{
	life -= dt;
	if (Alive())
		GameObject::Update(dt);
}



void CS230::Particle::Draw(DrawLayer drawlayer)
{
	if(Alive())
		if(shader == nullptr) GameObject::Draw();
		else {
			DrawSettings settings;
			settings.do_blending = true;

			CS230::DrawCall draw_call = {
				GetGOComponent<Sprite>()->GetTexture(),                       // Texture to draw
				&GetMatrix(),                          // Transformation matrix
				Engine::GetShaderManager().GetShader("change_alpha"), // Shader to use
				[this](const GLShader* shader) {
					SetAlpha(shader);
				},
				settings
			};

			GameObject::Draw(draw_call);
		}
}

void CS230::Particle::SetAlpha(const GLShader* shader)
{
	float alpha = float(life / max_life);
	shader->SendUniform("uAlpha", alpha);
}
