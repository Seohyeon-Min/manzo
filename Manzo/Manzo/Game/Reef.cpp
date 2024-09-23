#include "Reef.h"

Reef::Reef(Math::vec2 position) : CS230::GameObject(position)
{
	AddGOComponent(new CS230::Sprite("assets/images/Reef.spt", this));

}

void Reef::Update(double dt)
{
	GameObject::Update(dt);
}

void Reef::Draw(Math::TransformationMatrix camera_matrix) {
	CS230::GameObject::Draw(camera_matrix);
}