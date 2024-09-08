#include "Ship.h"

Ship::Ship(Math::vec2 start_position, GameObject* standing_on) :
	GameObject(start_position), moving(false)
{
	AddGOComponent(new CS230::Sprite("Assets/Robot.spt", this));
}

void Ship::Update(double dt)
{
}

void Ship::Draw(Math::TransformationMatrix camera_matrix)
{
}

bool Ship::CanCollideWith(GameObjectTypes)
{
	return false;
}

void Ship::ResolveCollision(GameObject* other_object)
{
}
