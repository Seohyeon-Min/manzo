#include "Inventory.h"
#include <iostream>
#include "Fish.h"

#include "../Engine/Engine.h"
#include "../Engine/Icon.h"
#include "Dragging.h"
#include "../Engine/GameObjectManager.h"
#include "FishEcosystem.h"
#include "PopUp.h"
#include "GameOption.h"

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


	Engine::GetIconManager().AddIcon("OpenInven", "money", "money", { 540,320 }, 1.0f, false);
	Engine::GetIconManager().AddIcon("OpenInven", "ModuleTab", "moduleTab", { 560, 220 }, 1.0f, false, false, true);
	Engine::GetIconManager().AddIcon("OpenInven", "FishTab", "fishTab", { 560,150 }, 1.0f, false, false, true);
	Engine::GetIconManager().AddIcon("OpenInven", "SpecialTab", "specialTab", { 560,80 }, 1.0f, false, false, true);

	/////////////////////////////// Module State //////////////////////////////////////
	Engine::GetIconManager().AddIcon("Module_Tab", "module_set1", "module_empty", { GetPosition().x + savePos[0].x, savePos[0].y }, 0.7f, false);
	Engine::GetIconManager().AddIcon("Module_Tab", "module_set2", "module_empty", { GetPosition().x + savePos[1].x, savePos[1].y }, 0.7f, false);

	Engine::GetIconManager().AddIcon("Module_Tab", "module_have1", "module_empty", { GetPosition().x + savePos[0].x, -savePos[0].y }, 0.7f, false);
	Engine::GetIconManager().AddIcon("Module_Tab", "module_have2", "module_empty", { GetPosition().x + savePos[1].x, -savePos[1].y }, 0.7f, false);

	Engine::GetIconManager().AddIcon("Module_Tab_Get", "module1", "module1_info", vec2(m1x, module_ptr->IsFirstSetted() ? savePos[0].y : -savePos[0].y), 1.f, true, true, true);
	Engine::GetIconManager().AddIcon("Module_Tab_Get", "module2", "module2_info", vec2(m2x, module_ptr->IsSecondSetted() ? savePos[1].y : -savePos[1].y), 1.f, true, true, true);

	/////////////////////////////// Fish Collection State //////////////////////////////////////
	Engine::GetIconManager().AddIcon("FishPopUp", "plus1", "plus1", { 80,-75 }, 1.f, false, false, true);
	Engine::GetIconManager().AddIcon("FishPopUp", "plus10", "plus10", { 50,-75 }, 1.f, false, false, true);

	Engine::GetIconManager().AddIcon("FishPopUp", "minus1", "minus1", { -80,-75 }, 1.f, false, false, true);
	Engine::GetIconManager().AddIcon("FishPopUp", "minus10", "minus10", { -50,-75 }, 1.f, false, false, true);
	Engine::GetIconManager().AddIcon("FishPopUp", "sell_fish", "confirm", { 0,-110 }, 1.f, false, false, true);

	if (!is_picked)  //pick today's special fish
	{
		if ((Engine::GetGameStateManager().GetPreviouseStateName() != "Tutorial" || Engine::GetGameStateManager().GetPreviouseStateName() != "Mode3"))
		{
			std::uniform_int_distribution<> todays_fish(0, total_fishNum - 1);
			std::uniform_int_distribution<> fish_price(1, 4);
			todays_fish_index = todays_fish(dre_todayFish);
			todays_price = fish_price(dre_price);

			is_picked = true;
		}
		else
		{
			//정보전달 어케 할건지? 셉뎉매 vs 겜뎉매
			is_picked = true;
		}
	}
	todays_fish_icon = "fish" + std::to_string(todays_fish_index + 1);
	Engine::GetIconManager().AddIcon("Mode2_Always", todays_fish_icon + "_today", todays_fish_icon, { -575,300 }, 1.0f, false, false, false, true, true);

	Engine::GetIconManager().ShowIconByGroup("Mode2_Always");

	for (auto& fish : originCollection)
	{
		if (fish.second != 0)
		{
			std::string file_name = "fish" + std::to_string(fish.first + 1);

			Engine::GetIconManager().AddIcon("Fish_Tab", file_name + "_having", file_name, { GetPosition().x + 100,float(p -= 80) }, 1.0f, true, false, true);
			Engine::GetIconManager().AddIcon("FishPopping", file_name + "_popping", file_name, { 0,40 }, 1.0f, false, false, false, false, false);
		}
	}
}

void Inven::Update(double dt)
{
	if (!Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->GetGOComponent<GameOption>()->isOpened())
	{
		GameObject::Update(dt);

		Icon* selectedIcon = nullptr;
		bool clicked = Engine::GetInput().MouseButtonJustPressed(SDL_BUTTON_LEFT);

		//std::cout << holding << std::endl;

		if (!holding && Engine::GetInput().MouseButtonJustPressed(SDL_BUTTON_LEFT)) {
			selectedIcon = Engine::GetIconManager().GetCollidingIconWithMouse({ Engine::GetInput().GetMousePos().mouseCamSpaceX ,Engine::GetInput().GetMousePos().mouseCamSpaceY });
			if (selectedIcon) {
				holding = true;
			}
		}

		if (selectedIcon != nullptr && clicked)
		{
			std::string alias = selectedIcon->GetId();

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

			holding = false;
		}

		if (current_state != &state_module)
		{
			Engine::GetIconManager().HideIconByGroup("Module_Tab");
			Engine::GetIconManager().HideIconByGroup("Module_Tab_Get");
		}

		if (current_state != &state_fc)
		{
			Engine::GetIconManager().HideIconByGroup("Fish_Tab");
			Engine::GetIconManager().HideIconByGroup("FishPopUp");
			how_much_sold = 1;

			for (auto& fish : originCollection)
			{
				if (fish.second != 0)
				{
					std::string file_name = "fish" + std::to_string(fish.first + 1) + "_having";

					Engine::GetIconManager().HideIconById(file_name);
				}
			}
		}

		if (!is_opened)
		{
			change_state(&state_none);
		}
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
		if (selectedIcon->GetId() == "can_go_shop" && clicked)
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

	Engine::GetIconManager().ShowIconByGroup("Module_Tab");
}

void Inven::State_Module::Update(GameObject* object, double dt)
{
	Inven* inven = static_cast<Inven*>(object);
	inven->GetGOComponent<Sprite>()->Update(dt);

	Engine::GetIconManager().ShowIconById("module_set1");

	/////////////////////////////////////////////////// Check Buy ///////////////////////////////////////////////////
	if (inven->buy_first_module)
	{
		Engine::GetIconManager().ShowIconById("module1");
	}

	if (inven->buy_second_module)
	{
		Engine::GetIconManager().ShowIconById("module2");
	}

	/////////////////////////////////////////////////// Check Set ///////////////////////////////////////////////////
	if (Engine::GetIconManager().IsCollidingWith("module1", "module_set1"))
	{
		inven->module_ptr->SetFirstModule(true);
		Engine::GetIconManager().SetIconPositionById("module1", "module_set1");
		inven->m1x = Engine::GetIconManager().GetIconPositionIfColliding("module_set1", "module1").x;
	}
	else if (Engine::GetIconManager().IsCollidingWith("module1", "module_set2"))
	{
		inven->module_ptr->SetFirstModule(true);
		Engine::GetIconManager().SetIconPositionById("module1", "module_set2");
		inven->m1x = Engine::GetIconManager().GetIconPositionIfColliding("module_set2", "module1").x;
	}
	else if(Engine::GetIconManager().IsCollidingWith("module1", "module_have1"))
	{
		inven->module_ptr->SetFirstModule(false);
		Engine::GetIconManager().SetIconPositionById("module1", "module_have1");
		inven->m1x = Engine::GetIconManager().GetIconPositionIfColliding("module_have1", "module1").x;
	}
	else if (Engine::GetIconManager().IsCollidingWith("module1", "module_have2"))
	{
		inven->module_ptr->SetFirstModule(false);
		Engine::GetIconManager().SetIconPositionById("module1", "module_have2");
		inven->m1x = Engine::GetIconManager().GetIconPositionIfColliding( "module_have2", "module1").x;
	}


	if (Engine::GetIconManager().IsCollidingWith("module2", "module_set1"))
	{
		inven->module_ptr->SetSecondModule(true);
		Engine::GetIconManager().SetIconPositionById("module2", "module_set1");
		inven->m2x = Engine::GetIconManager().GetIconPositionIfColliding("module_set1", "module2").x;
	}
	else if (Engine::GetIconManager().IsCollidingWith("module2", "module_set2"))
	{
		inven->module_ptr->SetSecondModule(true);
		Engine::GetIconManager().SetIconPositionById("module2", "module_set2");
		inven->m2x = Engine::GetIconManager().GetIconPositionIfColliding("module_set2", "module2").x;
	}
	else if (Engine::GetIconManager().IsCollidingWith("module2", "module_have1"))
	{
		inven->module_ptr->SetSecondModule(false);
		Engine::GetIconManager().SetIconPositionById("module2", "module_have1");
		inven->m2x = Engine::GetIconManager().GetIconPositionIfColliding("module_have1", "module2").x;
	}
	else if (Engine::GetIconManager().IsCollidingWith("module2", "module_have2"))
	{
		inven->module_ptr->SetSecondModule(false);
		Engine::GetIconManager().SetIconPositionById("module2", "module_have2");
		inven->m2x = Engine::GetIconManager().GetIconPositionIfColliding("module_have2", "module2").x;
	}
}

void Inven::State_Module::CheckExit(GameObject* object) {}

void Inven::State_FC::Enter(GameObject* object)
{
	Inven* inven = static_cast<Inven*>(object);
	inven->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::FishCollection));

	inven->page = 2;


	Engine::GetIconManager().ShowIconByGroup("Fish_Tab");

	Engine::GetIconManager().ShowIconById(inven->todays_fish_icon);
}

void Inven::State_FC::Update(GameObject* object, double dt)
{
	Inven* inven = static_cast<Inven*>(object);
	inven->GetGOComponent<Sprite>()->Update(dt);

	Icon* selectedIcon = Engine::GetIconManager().GetCollidingIconWithMouse({ Engine::GetInput().GetMousePos().mouseCamSpaceX ,Engine::GetInput().GetMousePos().mouseCamSpaceY });
	bool clicked = Engine::GetInput().MouseButtonJustPressed(SDL_BUTTON_LEFT);

	if (selectedIcon != nullptr)
	{
		std::string alias = selectedIcon->GetId();

		if (inven->fishCollection[inven->which_fish_sellected] > inven->how_much_sold && alias == "plus1" && clicked)
		{
			inven->how_much_sold++;

		}
		else if (inven->fishCollection[inven->which_fish_sellected] >= inven->how_much_sold + 10 && alias == "plus10" && clicked)
		{
			inven->how_much_sold += 10;
		}
		else if (inven->how_much_sold > 1 && alias == "minus1" && clicked)
		{
			inven->how_much_sold--;
		}
		else if (inven->how_much_sold >= 10 && alias == "minus10" && clicked)
		{
			inven->how_much_sold -= 10;
		}
	}
	else
	{
		inven->has_sold = false;
	}
}

void Inven::State_FC::CheckExit(GameObject* object)
{
	Inven* inven = static_cast<Inven*>(object);
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