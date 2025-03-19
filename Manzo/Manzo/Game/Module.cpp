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
	SetFirstModule(true);
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

SecondModule::SecondModule(Ship* ship) : ship(ship)
{
	SetSecondModule(true);
}

void SecondModule::Update(double dt)
{
	if (Module::IsSecondSetted())
	{
		Module::Update(dt);


		Engine::GetFontManager().PrintText(FontType::Bold, std::to_string(ship->GetPosition().x), ship->GetPosition(), 0.2f, {1.f,1.f,1.f}, 0.5f);
	}
}

void SecondModule::Draw(DrawLayer drawlayer)
{
	if (Module::IsSecondSetted())
	{
		Module::Draw(drawlayer);
	}
}
