#include "Inventory.h"
#include <iostream>
#include "Fish.h"

#include "../Engine/Engine.h"
#include "../Engine/Icon.h"
#include "Dragging.h"

Inven::Inven(vec2 position) : GameObject(position), page(0), dre_todayFish(rd()), dre_price(rd())
{
	AddGOComponent(new Sprite("assets/images/window.spt", this));
	change_state(&state_none);

	auto& saveData = Engine::GetSaveDataManager().GetSaveData();

	// 물고기
	fishCollection = saveData.fishCollection;
	originCollection = fishCollection;

	// 모듈 포인터
	module_ptr = Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->GetGOComponent<Module>();

	// 모듈 구매 여부
	buy_first_module = saveData.module1.buy;
	buy_second_module = saveData.module2.buy;

	// 모듈 장착 여부
	module_ptr->SetFirstModule(saveData.module1.set);
	module_ptr->SetSecondModule(saveData.module2.set);

	// 돈 및 위치
	money = saveData.money;
	m1x = saveData.module1.pos;
	m2x = saveData.module2.pos;
}

void Inven::Update(double dt)
{
	GameObject::Update(dt);
	Engine::GetIconManager().AddIcon("money", { 540,320 }, 1.0f, false);

	if (Engine::GetInput().KeyJustPressed(Input::Keys::Left) && is_opened)
	{
		page--;
	}
	else if (Engine::GetInput().KeyJustPressed(Input::Keys::Right) && is_opened)
	{
		page++;
	}

	if (is_opened && (current_state == &state_fc))
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
		Engine::GetIconManager().AddIcon(todays_fish_icon, { 0,250 }, 1.0f, false);
	}

	if (!is_opened)
	{
		Engine::GetIconManager().RemoveAllIcon();
	}
}

void Inven::Draw(DrawLayer drawlayer)
{
	if (is_opened)
	{
		Engine::GetIconManager().RemoveIcon("go_shop");
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

	Engine::GetIconManager().AddIcon("module_set", { inven->GetPosition().x + inven->savePos[0].x, inven->savePos[0].y }, 0.7f, false);
	Engine::GetIconManager().AddIcon("module_set", { inven->GetPosition().x + inven->savePos[1].x, inven->savePos[1].y }, 0.7f, false);

	Engine::GetIconManager().AddIcon("module_have", { inven->GetPosition().x + inven->savePos[0].x, -inven->savePos[0].y }, 0.7f, false);
	Engine::GetIconManager().AddIcon("module_have", { inven->GetPosition().x + inven->savePos[1].x, -inven->savePos[1].y }, 0.7f, false);
}

void Inven::State_Module::Update(GameObject* object, double dt)
{
	Inven* inven = static_cast<Inven*>(object);
	inven->GetGOComponent<Sprite>()->Update(dt);

	/////////////////////////////////////////////////// Check Buy ///////////////////////////////////////////////////
	if (inven->buy_first_module)
	{
		Engine::GetIconManager().AddIcon("module1", vec2(inven->m1x, inven->module_ptr->IsFirstSetted() ? inven->savePos[0].y : -inven->savePos[0].y), 0.7f, true, true, true);
	}

	if (inven->buy_second_module)
	{
		Engine::GetIconManager().AddIcon("module2", vec2(inven->m2x, inven->module_ptr->IsSecondSetted() ? inven->savePos[1].y : -inven->savePos[1].y), 0.7f, true, true, true);
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

void Inven::State_Module::CheckExit(GameObject* object)
{
	Inven* inven = static_cast<Inven*>(object);
	if (inven->page == 2)
	{
		Engine::GetIconManager().RemoveAllIcon();
		inven->change_state(&inven->state_fc);
	}

	if (!inven->is_opened)
	{
		inven->change_state(&inven->state_none);
	}
}

void Inven::State_FC::Enter(GameObject* object)
{
	int position = 100;
	Inven* inven = static_cast<Inven*>(object);
	inven->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::FishCollection));
	inven->in_fish_state = true;

	for (auto& fish : inven->originCollection)
	{
		if (fish.second != 0)
		{
			std::string file_name = "fish" + std::to_string(fish.first + 1);

			//0일땐 드래그 못하게 할지? 그리고 약간 몇 마리 남아있는지 짜치지 않나...좀 얘기해봐야할듯
			Engine::GetIconManager().AddIcon(file_name, { inven->GetPosition().x + 100,float(position -= 80) }, 1.0f, true, false, true);
		}
	}

	Engine::GetIconManager().AddIcon("plus1", { 80,180 }, 1.f, false, false, true);
	Engine::GetIconManager().AddIcon("plus10", { 50,180 }, 1.f, false, false, true);

	Engine::GetIconManager().AddIcon("minus1", { -80,180 }, 1.f, false, false, true);
	Engine::GetIconManager().AddIcon("minus10", { -50,180 }, 1.f, false, false, true);
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
	if (inven->page == 1)
	{
		Engine::GetIconManager().RemoveAllIcon();
		inven->change_state(&inven->state_module);
		inven->in_fish_state = false;
	}
	else if (inven->page == 3)
	{
		Engine::GetIconManager().RemoveAllIcon();
		inven->change_state(&inven->state_sc);
		inven->in_fish_state = false;
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
		Engine::GetIconManager().RemoveAllIcon();
		inven->change_state(&inven->state_fc);
	}

	if (!inven->is_opened)
	{
		Engine::GetIconManager().RemoveAllIcon();
		inven->change_state(&inven->state_none);
	}
}