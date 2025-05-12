#include "Shop.h"
#include "Dragging.h"
#include "..\Engine\GameObject.h"
#include "Inventory.h"

static bool is_on_inven = false;
static bool is_on_shop = false;
static bool Ready_to_buy = false;
static bool Ready_to_sell = false;

Shop::Shop(vec2 postion) : GameObject(postion)
{
	Read_Shop_Csv("assets/scenes/shop.csv");
	AddGOComponent(new Sprite("assets/images/Price.spt", this));

	int count = 1;
	for (auto& info : shop_infos)
	{
		++count;
		is_on_shop = true;

		/*Engine::GetIconManager().AddIcon(
				info.icon,
				{ -500.0f, 300.0f - (80.0f * count) },
				1.f
			);*/
	}

	inven = Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->GetGOComponent<Inven>();
}

Shop::~Shop()
{
}


void Shop::Update(double dt)
{
	GameObject::Update(dt);

	if (inven->GetIsOpened())
	{
		for (auto& info : shop_infos)
		{
			std::string icon_name = info.icon;

			Engine::GetIconManager().ShowIcon(icon_name);

			if (Engine::GetIconManager().IsCollidingWith(icon_name, "module_have") && inven->GetMoney() >= info.price)
			{
				vec2 drop_pos = Engine::GetIconManager().GetIconPosition("module_have", icon_name);

				if (drop_pos.x == 350 + inven->savePos[0].x)
				{
					inven->BuyFirstModule(true);
				}
				else if (drop_pos.x == 350 + inven->savePos[1].x)
				{
					inven->BuySecondModule(true);
				}

				//Engine::GetIconManager().RemoveIcon(icon_name);

				if (!already_buy)
				{
					already_buy = true;
					inven->SetMoney(inven->GetMoney() - info.price);
				}
			}
		}
	}
	else
	{
		for (auto& info : shop_infos)
		{
			Engine::GetIconManager().HideIcon(info.icon);
		}
	}
}

void Shop::Draw(DrawLayer drawlayer)
{
	if(inven->GetIsOpened())
	{
		GameObject::Draw();
	}
}

void Shop::Read_Shop_Csv(const std::string& filename)
{
	std::ifstream file(filename);
	if (!file.is_open()) 
	{
		std::cerr << "Failed to open file: " << filename << std::endl;
		return;
	}

	std::string line, cell;
	std::getline(file, line);

	while (std::getline(file, line)) 
	{
		std::stringstream linestream(line);
		shop_info shop_i;

		std::getline(linestream, cell, ',');
		shop_i.name = cell;

		std::getline(linestream, cell, ',');
		shop_i.icon = cell;

		std::getline(linestream, cell, ',');
		shop_i.price = static_cast<int>(std::stol(cell));

		std::getline(linestream, cell, ',');
		shop_i.script = cell;
		
		shop_infos.push_back(shop_i);
	}
	file.close();
	std::cout << "Shop loaded successfully." << std::endl;
}

bool Shop::CanCollideWith(GameObjectTypes other_object)
{
	return (other_object == GameObjectTypes::Icon);
}