#include "Module.h"

Module::Module(vec2 start_position, Ship* ship) : GameObject(start_position), ship(ship)
{
	AddGOComponent(new Sprite("assets/images/module1.spt", this));
}

void Module::Update(double dt)
{
	GameObject::Update(dt);

	SetPosition(ship->GetDashPos());
}

void Module::Draw(DrawLayer drawlayer)
{
	GameObject::Draw(drawlayer);
}

void Module::State_First_Module::Enter(GameObject* object)
{
}

void Module::State_First_Module::Update(GameObject* object, double dt)
{
}

void Module::State_First_Module::CheckExit(GameObject* object)
{
}
