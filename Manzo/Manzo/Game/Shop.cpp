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

		std::string img_type = (info.icon.find_first_of("0123456789") != std::string::npos) ? info.icon : "module_info";

		Engine::GetIconManager().AddIcon("Shop",
				info.icon, img_type,
				{ -500.0f, 300.0f - (80.0f * count) },
				1.f
			);
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
		Engine::GetIconManager().ShowIconByGroup("Shop");
		for (auto& info : shop_infos)
		{
			std::string icon_name = info.icon;


			/*if (Engine::GetIconManager().IsCollidingWith(icon_name, "module_have") && inven->GetMoney() >= info.price)
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

				if (!already_buy)
				{
					already_buy = true;
					inven->SetMoney(inven->GetMoney() - info.price);
				}
			}*/
		}
	}
	else
	{
		Engine::GetIconManager().HideIconByGroup("Shop");
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