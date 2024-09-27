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

void CS230::Particle::Start(vec2 position, vec2 velocity, double max_life)
{
	SetPosition(position);
	SetVelocity(velocity);
	life = max_life;
	GetGOComponent<CS230::Sprite>()->Reset();
}

void CS230::Particle::Update(double dt)
{
	life -= dt;
	if (Alive())
		GameObject::Update(dt);
}

void CS230::Particle::Draw()
{
	if(Alive())
		GameObject::Draw();
}
