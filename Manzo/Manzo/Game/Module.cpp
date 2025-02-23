#include "Module.h"

Module::Module() : GameObject({ 0,0 }) { }

Module::Module(vec2 start_position) : GameObject(start_position) { }

void Module::Update(double dt) 
{
	GameObject::Update(dt);
}

void Module::Draw(DrawLayer drawlayer)
{
	if (Engine::GetGameStateManager().GetStateName() == "Mode1")
	{
		GameObject::Draw(drawlayer);
	}
}


// First Module

FirstModule::FirstModule(Ship* ship) : ship(ship)
{
	AddGOComponent(new Sprite("assets/images/module1.spt", this));
}

void FirstModule::Update(double dt)
{
	if (Module::IsFirstSetted())
	{
		Module::Update(dt);
		SetPosition(ship->GetDashPos());
	}
}

void FirstModule::Draw(DrawLayer drawlayer)
{
	if (Module::IsFirstSetted())
	{
		Module::Draw(drawlayer);
	}
}