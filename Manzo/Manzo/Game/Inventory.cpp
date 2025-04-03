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

	fishCollection = Engine::GetLogger().GetFishCollection();

	module_ptr = Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->GetGOComponent<Module>();
	module_ptr->SetFirstModule(Engine::GetLogger().GetModule1());
	module_ptr->SetSecondModule(Engine::GetLogger().GetModule2());
	money = Engine::GetLogger().GetMoney();
	m1x = Engine::GetLogger().GetModule1XPos();
	m2x = Engine::GetLogger().GetModule2XPos();
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
		Engine::GetIconManager().AddIcon(todays_fish_icon, { GetPosition().x,250}, 1.0f, false);
	}
	else
	{
		//today fish 아이콘만 지우기
		//Engine::GetIconManager().RemoveIcon(todays_fish_icon);
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
		if(selectedIcon->GetAlias() == "go_shop" && clicked)
			return true;
	}
	return false;
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

	Engine::GetIconManager().AddIcon("module", { inven->GetPosition().x + inven->savePos[0].x, inven->savePos[0].y }, 0.7f, false);
	Engine::GetIconManager().AddIcon("module", { inven->GetPosition().x + inven->savePos[1].x, inven->savePos[1].y }, 0.7f, false);
	//Engine::GetIconManager().AddIcon("module", inven->savePos[2], 0.7f, false);

	Engine::GetIconManager().AddIcon(
		"module1",
		(inven->module_ptr->IsFirstSetted()) ? vec2((float)inven->GetPosition().x + inven->m1x, 100) : vec2(inven->GetPosition().x -130, -100),
		0.7f, true, true, true
	);

	Engine::GetIconManager().AddIcon(
		"module2",
		(inven->module_ptr->IsSecondSetted()) ? vec2((float)inven->GetPosition().x - inven->m2x, 100) : vec2(inven->GetPosition().x, -100),
		0.7f, true, true, true
	);

	//Engine::GetIconManager().AddIcon("module3", { 130,-100 }, 0.7f, true, true, true);
}

void Inven::State_Module::Update(GameObject* object, double dt)
{
	Inven* inven = static_cast<Inven*>(object);
	inven->GetGOComponent<Sprite>()->Update(dt);

	if (Engine::GetIconManager().IsCollidingWith("module", "module1"))
	{
		inven->module_ptr->SetFirstModule(true);
		inven->m1x = Engine::GetIconManager().GetIconPosition("module", "module1").x;
	}
	else
	{
		inven->module_ptr->SetFirstModule(false);
		inven->m1x = -130;
	}

	if (Engine::GetIconManager().IsCollidingWith("module", "module2"))
	{
		inven->module_ptr->SetSecondModule(true);
		inven->m2x = Engine::GetIconManager().GetIconPosition("module", "module2").x;
	}
	else
	{
		inven->module_ptr->SetSecondModule(false);
		inven->m2x = 0;
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
	int position = -100;
	Inven* inven = static_cast<Inven*>(object);
	inven->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::FishCollection));

	for (auto& fish : inven->fishCollection)
	{
		std::string file_name = "fish" + std::to_string(fish.first + 1);		
		Engine::GetIconManager().AddIcon(file_name, { inven->GetPosition().x + 100,float(position += 80) }, 1.0f, false);
	}

	Engine::GetIconManager().AddIcon("plus1", { inven->GetPosition().x + 80,180 }, 1.f, false, false, true);
	Engine::GetIconManager().AddIcon("plus10", { inven->GetPosition().x + 50,180 }, 1.f, false, false, true);

	Engine::GetIconManager().AddIcon("minus1", { inven->GetPosition().x - 80,180 }, 1.f, false, false, true);
	Engine::GetIconManager().AddIcon("minus10", { inven->GetPosition().x - 50,180 }, 1.f, false, false, true);
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
		Engine::GetIconManager().RemoveIcon("fish" + std::to_string(inven->todays_fish_index + 1));
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
	}

	// decide to sell
	if (Engine::GetInput().KeyJustPressed(Input::Keys::F1))
	{
		inven->money += (inven->todays_price * inven->how_much_sold);
		inven->fishCollection[inven->todays_fish_index] -= inven->how_much_sold;

		if (inven->fishCollection[inven->todays_fish_index] == 0)
		{
			Engine::GetIconManager().RemoveIcon("fish"+ std::to_string(inven->todays_fish_index + 1));
		}
	}

}

void Inven::State_FC::CheckExit(GameObject* object)
{
	Inven* inven = static_cast<Inven*>(object);
	if (inven->page == 1)
	{
		Engine::GetIconManager().RemoveAllIcon();
		inven->change_state(&inven->state_module);
	}
	else if (inven->page == 3)
	{
		Engine::GetIconManager().RemoveAllIcon();
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
		Engine::GetIconManager().RemoveAllIcon();
		inven->change_state(&inven->state_fc);
	}

	if (!inven->is_opened)
	{
		Engine::GetIconManager().RemoveAllIcon();
		inven->change_state(&inven->state_none);
	}
}