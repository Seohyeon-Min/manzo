#include "Inventory.h"
#include <iostream>
#include "Fish.h"

#include "../Engine/Engine.h"

Inven::Inven(vec2 position) : GameObject(position), page(0), dre_todayFish(rd()), dre_price(rd())
{
	AddGOComponent(new Sprite("assets/images/window.spt", this));
	change_state(&state_none);
}

void Inven::Update(double dt)
{
	GameObject::Update(dt);

	if (Engine::GetInput().KeyJustPressed(Input::Keys::Left) && is_opened)
	{
		page--;
	}
	else if (Engine::GetInput().KeyJustPressed(Input::Keys::Right) && is_opened)
	{
		page++;
	}

	// 
	if (is_opened)
	{
		if (!is_picked)
		{
			std::uniform_int_distribution<> todays_fish(0, total_fishNum - 1);
			std::uniform_int_distribution<> fish_price(1, 4);
			todays_fish_index = todays_fish(dre_todayFish);
			todays_price = fish_price(dre_price);

			std::cout << "Today's fish is : " << todays_fish_index << ",   price : " << todays_price << "\n";
			is_picked = true;
		}
		todays_fish_icon = "fish" + std::to_string(todays_fish_index + 1);
		Engine::GetIconManager().AddIcon(todays_fish_icon, { -200,0 }, 1.0f, false);
	}
	else
	{
		Engine::GetIconManager().RemoveAllIcon();
	}
}

void Inven::Draw(DrawLayer drawlayer)
{
	if (is_opened)
	{
		GameObject::Draw();
	}
}

void Inven::ReadSaveFile(const std::string& filename)
{
	std::ifstream file(filename);
	if (!file.is_open()) {
		std::cerr << "Failed to open file: " << filename << std::endl;
		return;
	}

	fishCollection.clear();
	int fishType, count;
	std::string line;

	while (std::getline(file, line)) {
		std::istringstream ss(line);
		if (ss >> fishType >> count) {
			fishCollection[fishType - 1] = count;
		}
		else if (line.find("Money:") != std::string::npos) {
			std::istringstream ss_money(line.substr(7));
			ss_money >> money;
		}
	}

	file.close();
}

void Inven::State_None::Enter(GameObject* object)
{
	Inven* inven = static_cast<Inven*>(object);
	inven->is_picked = false;
	inven->is_opened = false;
	inven->page = 0;
}

void Inven::State_None::Update(GameObject* object, double dt)
{
}

void Inven::State_None::CheckExit(GameObject* object)
{
	Inven* inven = static_cast<Inven*>(object);
	if (inven->is_opened)
	{
		inven->change_state(&inven->state_module);
	}
}

void Inven::State_Module::Enter(GameObject* object)
{
	Inven* inven = static_cast<Inven*>(object);
	inven->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Module));
	inven->page = 1;
}

void Inven::State_Module::Update(GameObject* object, double dt)
{
	Inven* inven = static_cast<Inven*>(object);
	inven->GetGOComponent<Sprite>()->Update(dt);
}

void Inven::State_Module::CheckExit(GameObject* object)
{
	Inven* inven = static_cast<Inven*>(object);
	if (inven->page == 2)
	{
		inven->change_state(&inven->state_fc);
	}

	if (!inven->is_opened)
	{
		inven->change_state(&inven->state_none);
	}
}

void Inven::State_FC::Enter(GameObject* object)
{
	int position = -100;
	Inven* inven = static_cast<Inven*>(object);
	inven->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::FishCollection));
	inven->ReadSaveFile("save_data.txt");

	for (auto& fish : inven->fishCollection)
	{
		std::string file_name = "fish" + std::to_string(fish.first+1);
		Engine::GetIconManager().AddIcon(file_name, { 100,float(position += 80) }, 1.0f);
	}
}

void Inven::State_FC::Update(GameObject* object, double dt)
{
	Inven* inven = static_cast<Inven*>(object);
	inven->GetGOComponent<Sprite>()->Update(dt);

	// Sell system
	if (Engine::GetInput().KeyJustPressed(Input::Keys::O))
	{
		//decrease each type of fish
		inven->fishCollection[inven->todays_fish_index]--;

		//earn money
		inven->money += inven->todays_price;
	}

	//std::cout << inven->money << std::endl;

	// fish icons in collection
}

void Inven::State_FC::CheckExit(GameObject* object)
{
	Inven* inven = static_cast<Inven*>(object);
	if (inven->page == 1)
	{
		inven->change_state(&inven->state_module);
	}
	else if (inven->page == 3)
	{
		inven->change_state(&inven->state_sc);
	}

	if (!inven->is_opened)
	{
		inven->change_state(&inven->state_none);
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
	if (inven->page == 2)
	{
		inven->change_state(&inven->state_fc);
	}

	if (!inven->is_opened)
	{
		inven->change_state(&inven->state_none);
	}
}