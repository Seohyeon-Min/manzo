#include "Inventory.h"
#include <iostream>

Inven::Inven(vec2 position) : GameObject(position), page(0)
{
	AddGOComponent(new Sprite("assets/images/window.spt", this));
	change_state(&state_module);
}

void Inven::Update(double dt)
{
	GameObject::Update(dt);

	if (Engine::GetInput().KeyJustPressed(Input::Keys::Left))
	{
		page--;
	}
	else if (Engine::GetInput().KeyJustPressed(Input::Keys::Right))
	{
		page++;
	}

	std::cout << page << std::endl;
}

void Inven::Draw(DrawLayer drawlayer)
{
	if (is_opened)
	{
		GameObject::Draw();
	}
}

void Inven::State_Module::Enter(GameObject* object)
{
	Inven* inven = static_cast<Inven*>(object);
	inven->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Module));
}

void Inven::State_Module::Update(GameObject* object, double dt)
{
	Inven* inven = static_cast<Inven*>(object);
	inven->GetGOComponent<Sprite>()->Update(dt);
}

void Inven::State_Module::CheckExit(GameObject* object)
{
	Inven* inven = static_cast<Inven*>(object);
	if (inven->page == 1)
	{
		inven->change_state(&inven->state_fc);
	}
}

void Inven::State_FC::Enter(GameObject* object)
{
	Inven* inven = static_cast<Inven*>(object);
	inven->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::FishCollection));
}

void Inven::State_FC::Update(GameObject* object, double dt)
{
	Inven* inven = static_cast<Inven*>(object);
	inven->GetGOComponent<Sprite>()->Update(dt);
}

void Inven::State_FC::CheckExit(GameObject* object)
{
	Inven* inven = static_cast<Inven*>(object);
	if (inven->page == 0)
	{
		inven->change_state(&inven->state_module);
	}
	else if (inven->page == 2)
	{
		inven->change_state(&inven->state_sc);
	}
}

void Inven::State_SC::Enter(GameObject* object)
{
	Inven* inven = static_cast<Inven*>(object);
	inven->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::SpecialCollection));
}

void Inven::State_SC::Update(GameObject* object, double dt)
{
	Inven* inven = static_cast<Inven*>(object);
	inven->GetGOComponent<Sprite>()->Update(dt);
}

void Inven::State_SC::CheckExit(GameObject* object)
{
	Inven* inven = static_cast<Inven*>(object);
	if (inven->page == 1)
	{
		inven->change_state(&inven->state_fc);
	}
}