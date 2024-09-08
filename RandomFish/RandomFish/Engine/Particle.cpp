#include "Particle.h"

CS230::Particle::Particle(const std::filesystem::path& sprite_file) : GameObject({0,0}), sprite(sprite_file, this)
{
	life = 0.0;
	sprite.Load(sprite_file, this);
	GameObject::AddGOComponent(&sprite);
}

void CS230::Particle::Start(Math::vec2 position, Math::vec2 velocity, double max_life)
{
	SetPosition(position);
	SetVelocity(velocity);
	life = max_life;
	sprite.PlayAnimation(0);
}

void CS230::Particle::Update(double dt)
{
	if (Alive())
	{
		life--;
		GameObject::Update(dt);
		sprite.Update(dt);
	}
}

void CS230::Particle::Draw(Math::TransformationMatrix camera_matrix)
{
	if (Alive())
	{
		GameObject::Draw(camera_matrix);
	}
}

