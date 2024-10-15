#include "Reef.h"
#include "Ship.h"
#include "../Engine/MapManager.h"

Reef::Reef(vec2 position)
    : GameObject(position)
{
	AddGOComponent(new CS230::Sprite("assets/images/Reef.spt", this));
}


void Reef::Update(double dt)
{
	GameObject::Update(dt);
}

void Reef::Draw() {
	CS230::GameObject::Draw();
}
