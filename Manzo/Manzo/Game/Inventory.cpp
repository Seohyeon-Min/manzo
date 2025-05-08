#include "Inventory.h"
#include <iostream>
#include "Fish.h"

#include "../Engine/Engine.h"
#include "../Engine/Icon.h"
#include "Dragging.h"
#include "../Engine/GameObjectManager.h"

Inven::Inven(vec2 position) : GameObject(position), dre_todayFish(rd()), dre_price(rd())
{
	AddGOComponent(new Sprite("assets/images/window.spt", this));
	change_state(&state_none);

	auto& saveData = Engine::GetSaveDataManager().GetSaveData();

	fishCollection = saveData.fishCollection;
	originCollection = fishCollection;

	module_ptr = Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->GetGOComponent<Module>();

	buy_first_module = saveData.module1.buy;
	buy_second_module = saveData.module2.buy;

	module_ptr->SetFirstModule(saveData.module1.set);
	module_ptr->SetSecondModule(saveData.module2.set);

	money = saveData.money;
	m1x = saveData.module1.pos;
	m2x = saveData.module2.pos;

	Engine::GetIconManager().AddIcon("money", { 540,320 }, 1.0f, false);

	Engine::GetIconManager().AddIcon("ModuleTab", { 560,220 }, 1.0f, false, false, true);
	Engine::GetIconManager().AddIcon("FishTab", { 560,150 }, 1.0f, false, false, true);
	Engine::GetIconManager().AddIcon("SpecialTab", { 560,80 }, 1.0f, false, false, true);

	///////////////////////////// Module State //////////////////////////////////////
	Engine::GetIconManager().AddIcon("module_set", { GetPosition().x + savePos[0].x, savePos[0].y }, 0.7f, false);
	Engine::GetIconManager().AddIcon("module_set", { GetPosition().x + savePos[1].x, savePos[1].y }, 0.7f, false);

	Engine::GetIconManager().AddIcon("module_have", { GetPosition().x + savePos[0].x, -savePos[0].y }, 0.7f, false);
	Engine::GetIconManager().AddIcon("module_have", { GetPosition().x + savePos[1].x, -savePos[1].y }, 0.7f, false);

	Engine::GetIconManager().AddIcon("module1", vec2(m1x, module_ptr->IsFirstSetted() ? savePos[0].y : -savePos[0].y), 1.f, true, true, true);
	Engine::GetIconManager().AddIcon("module2", vec2(m2x, module_ptr->IsSecondSetted() ? savePos[1].y : -savePos[1].y), 1.f, true, true, true);
	
	///////////////////////////// Fish Collection State //////////////////////////////////////
	Engine::GetIconManager().AddIcon("plus1", { 80,-75 }, 1.f, false, false, true);
	Engine::GetIconManager().AddIcon("plus10", { 50,-75 }, 1.f, false, false, true);

	Engine::GetIconManager().AddIcon("minus1", { -80,-75 }, 1.f, false, false, true);
	Engine::GetIconManager().AddIcon("minus10", { -50,-75 }, 1.f, false, false, true);

	if (!is_picked)  //pick today's special fish
	{
		std::uniform_int_distribution<> todays_fish(0, total_fishNum - 1);
		std::uniform_int_distribution<> fish_price(1, 4);
		todays_fish_index = todays_fish(dre_todayFish);
		todays_price = fish_price(dre_price);

		std::cout << "Today's fish is : " << todays_fish_index << ",   price : " << todays_price << "\n";
		is_picked = true;
	}
	todays_fish_icon = "fish" + std::to_string(todays_fish_index + 1);
	Engine::GetIconManager().AddIcon(todays_fish_icon, { -575,300 }, 1.0f, false, false, false, true, true);


	for (auto& fish : originCollection)
	{
		if (fish.second != 0)
		{
			std::string file_name = "fish" + std::to_string(fish.first + 1);
			std::string file_name_pop = "fish_pop" + std::to_string(fish.first + 1);

			Engine::GetIconManager().AddIcon(file_name, { GetPosition().x + 100,float(p -= 80) }, 1.0f, true, false, true);
			Engine::GetIconManager().AddIcon(file_name_pop, { 0,40 }, 1.0f, false, false, false, false, false);
		}
	}
}

void Inven::Update(double dt)
{
	GameObject::Update(dt);

	Icon* selectedIcon = Engine::GetIconManager().GetCollidingIconWithMouse({ Engine::GetInput().GetMousePos().mouseCamSpaceX ,Engine::GetInput().GetMousePos().mouseCamSpaceY });
	bool clicked = Engine::GetInput().MouseButtonJustPressed(SDL_BUTTON_LEFT);

	if (selectedIcon != nullptr && clicked)
	{
		std::string alias = selectedIcon->GetAlias();

		if (alias == "ModuleTab")
		{
			page = 1;
			change_state(&state_module);
		}
		else if (alias == "FishTab")
		{
			page = 2;
			change_state(&state_fc);
		}
		else if (alias == "SpecialTab")
		{
			page = 3;
			change_state(&state_sc);
		}
		else if (alias == "close_icon")
		{
			page = 0;
			change_state(&state_none);
			is_opened = false;
		}
	}

	if (current_state != &state_module)
	{
		Engine::GetIconManager().HideIcon("module1");
		Engine::GetIconManager().HideIcon("module2");
		Engine::GetIconManager().HideIcon("module_set");
		Engine::GetIconManager().HideIcon("module_have");
	}

	if (current_state != &state_fc)
	{
		Engine::GetIconManager().HideIcon("plus1");
		Engine::GetIconManager().HideIcon("plus10");
		Engine::GetIconManager().HideIcon("minus1");
		Engine::GetIconManager().HideIcon("minus10");
		Engine::GetIconManager().HideIcon("close_icon");
		//Engine::GetIconManager().HideIcon(todays_fish_icon);

		for (auto& fish : originCollection)
		{
			if (fish.second != 0)
			{
				std::string file_name = "fish" + std::to_string(fish.first + 1);

				Engine::GetIconManager().HideIcon(file_name);
			}
		}
	}

	if (!is_opened)
	{
		change_state(&state_none);
		//Engine::GetIconManager().RemoveAllIcon();
	}
}

void Inven::Draw(DrawLayer drawlayer)
{
	if (is_opened)
	{
		GameObject::Draw();
	}
}

bool Inven::Open()
{
	Icon* selectedIcon = Engine::GetIconManager().GetCollidingIconWithMouse({ Engine::GetInput().GetMousePos().mouseCamSpaceX ,Engine::GetInput().GetMousePos().mouseCamSpaceY });
	bool clicked = Engine::GetInput().MouseButtonJustPressed(SDL_BUTTON_LEFT);

	if (selectedIcon != nullptr)
	{
		if (selectedIcon->GetAlias() == "go_shop" && clicked)
			return true;
	}
	return false;
}

void Inven::State_None::Enter(GameObject* object)
{
	Inven* inven = static_cast<Inven*>(object);

	//select random index at each time when inventory is opened
	//inven->is_picked = false;

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

	Engine::GetIconManager().ShowIcon("module_set");
	Engine::GetIconManager().ShowIcon("module_have");
}

void Inven::State_Module::Update(GameObject* object, double dt)
{
	Inven* inven = static_cast<Inven*>(object);
	inven->GetGOComponent<Sprite>()->Update(dt);

	/////////////////////////////////////////////////// Check Buy ///////////////////////////////////////////////////
	if (inven->buy_first_module)
	{
		Engine::GetIconManager().ShowIcon("module1");
	}

	if (inven->buy_second_module)
	{
		Engine::GetIconManager().ShowIcon("module2");
	}

	/////////////////////////////////////////////////// Check Set ///////////////////////////////////////////////////
	if (Engine::GetIconManager().IsCollidingWith("module_set", "module1"))
	{
		inven->module_ptr->SetFirstModule(true);
		inven->m1x = Engine::GetIconManager().GetIconPosition("module_set", "module1").x;
	}
	else
	{
		inven->module_ptr->SetFirstModule(false);
		inven->m1x = inven->GetPosition().x + inven->savePos[0].x;
	}

	if (Engine::GetIconManager().IsCollidingWith("module_set", "module2"))
	{
		inven->module_ptr->SetSecondModule(true);
		inven->m2x = Engine::GetIconManager().GetIconPosition("module_set", "module2").x;
	}
	else
	{
		inven->module_ptr->SetSecondModule(false);
		inven->m2x = inven->GetPosition().x + inven->savePos[1].x;
	}


	/////////////////////////////////////////////////// Clear ///////////////////////////////////////////////////
	if (Engine::GetIconManager().IsCollidingWith("module_have", "module1"))
	{
		inven->module_ptr->SetFirstModule(false);
		inven->m1x = inven->GetPosition().x + inven->savePos[0].x;
	}

	if (Engine::GetIconManager().IsCollidingWith("module_have", "module2"))
	{
		inven->module_ptr->SetSecondModule(false);
		inven->m2x = inven->GetPosition().x + inven->savePos[1].x;
	}
}

void Inven::State_Module::CheckExit(GameObject* object){}

void Inven::State_FC::Enter(GameObject* object)
{
	Inven* inven = static_cast<Inven*>(object);
	inven->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::FishCollection));

	inven->page = 2;

	for (auto& fish : inven->originCollection)
	{
		if (fish.second != 0)
		{
			std::string file_name = "fish" + std::to_string(fish.first + 1);

			Engine::GetIconManager().ShowIcon(file_name);
		}
	}

	//Engine::GetIconManager().ShowIcon(inven->todays_fish_icon);
}

void Inven::State_FC::Update(GameObject* object, double dt)
{
	Inven* inven = static_cast<Inven*>(object);
	inven->GetGOComponent<Sprite>()->Update(dt);

	// Sell all
	if (Engine::GetInput().KeyJustPressed(Input::Keys::O))
	{
		//earn money
		inven->money += (inven->todays_price * inven->fishCollection[inven->todays_fish_index]);

		//decrease each type of fish
		inven->fishCollection[inven->todays_fish_index] = 0;
		inven->how_much_sold = 0;
	}

	Icon* selectedIcon = Engine::GetIconManager().GetCollidingIconWithMouse({ Engine::GetInput().GetMousePos().mouseCamSpaceX ,Engine::GetInput().GetMousePos().mouseCamSpaceY });
	bool clicked = Engine::GetInput().MouseButtonJustPressed(SDL_BUTTON_LEFT);

	if (selectedIcon != nullptr)
	{
		std::string alias = selectedIcon->GetAlias();
		if (inven->fishCollection[inven->todays_fish_index] > inven->how_much_sold && alias == "plus1" && clicked)
		{
			inven->how_much_sold++;
		}
		else if (inven->fishCollection[inven->todays_fish_index] >= inven->how_much_sold + 10 && alias == "plus10" && clicked)
		{
			inven->how_much_sold += 10;
		}
		else if (inven->how_much_sold > 0 && alias == "minus1" && clicked)
		{
			inven->how_much_sold--;
		}
		else if (inven->how_much_sold >= 10 && alias == "minus10" && clicked)
		{
			inven->how_much_sold -= 10;
		}

		// sell basic fishes
		for (auto& fish : inven->originCollection)
		{
			int index = fish.first;
			int count = fish.second;

			if (count != 0 && index != inven->todays_fish_index)
			{
				std::string fish_alias = "fish" + std::to_string(index + 1);

				if (alias == fish_alias &&
					Engine::GetIconManager().IsCollidingWith(fish_alias, "rect1") &&
					inven->fishCollection[index] != 0 &&
					!inven->has_sold)
				{
					inven->has_sold = true;
					inven->fishCollection[index] -= 1;
					inven->money += 1;
				}
			}
		}
	}
	else
	{
		inven->has_sold = false;
	}

	// decide to sell
	if (Engine::GetInput().KeyJustPressed(Input::Keys::F1) && inven->fishCollection[inven->todays_fish_index] != 0)
	{
		inven->money += (inven->todays_price * inven->how_much_sold);
		inven->fishCollection[inven->todays_fish_index] -= inven->how_much_sold;
		inven->how_much_sold = 0;
	}
}

void Inven::State_FC::CheckExit(GameObject* object)
{
	Inven* inven = static_cast<Inven*>(object);
	inven->how_much_sold = 0;
}

void Inven::State_SC::Enter(GameObject* object)
{
	Inven* inven = static_cast<Inven*>(object);
	inven->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::SpecialCollection));
	inven->page = 3;
}

void Inven::State_SC::Update(GameObject* object, double dt)
{
	Inven* inven = static_cast<Inven*>(object);
	inven->GetGOComponent<Sprite>()->Update(dt);
}

void Inven::State_SC::CheckExit(GameObject* object)
{
}