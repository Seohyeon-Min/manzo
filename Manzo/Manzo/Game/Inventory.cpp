#include "Inventory.h"
#include <iostream>
#include "Fish.h"

Inven::Inven(vec2 position) : GameObject(position), page(0)
{
	AddGOComponent(new Sprite("assets/images/window.spt", this));
	change_state(&state_module);
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

	/*if (!is_opened)
	{
		change_state(&);
	}*/
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
	inven->ReadSaveFile("save_data.txt");
}

void Inven::State_FC::Update(GameObject* object, double dt)
{
	Inven* inven = static_cast<Inven*>(object);
	inven->GetGOComponent<Sprite>()->Update(dt);

	/*for (auto f : inven->fishCollection)
	{
		std::cout << f.first << ": " << f.second << std::endl;
	}*/

	// Sell system
	if (Engine::GetInput().KeyJustPressed(Input::Keys::O)) {
		//decrease each type of fish
		inven->fishCollection[0]--;
		//earn money
		inven->money++;
	}

	//std::cout << inven->money << std::endl;
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