#include "Rock.h"
#include <iostream>

Rock::Rock(Math::vec2 start_position) :
	GameObject(start_position)
{
	AddGOComponent(new CS230::Sprite("Assets/ship.spt", this));
	SetVelocity({ 0,0 });
}

void Rock::Update(double dt)
{
	GameObject::Update(dt);
}


void Rock::Draw(Math::TransformationMatrix camera_matrix)
{
	GameObject::Draw(camera_matrix);
}

bool Rock::CanCollideWith(GameObjectTypes)
{
	return false;
}

void Rock::ResolveCollision(GameObject* other_object)
{
}
