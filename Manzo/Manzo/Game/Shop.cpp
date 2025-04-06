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
		std::cout << count << "th: " << std::endl;
		std::cout << "name: " << info.name << std::endl;
		std::cout << "icon name: " << info.icon << std::endl;
		std::cout << "price : " << info.price << std::endl;
		std::cout << "script : " << info.script << std::endl;
		++count;
		is_on_shop = true;
	}
}

Shop::~Shop()
{
}


void Shop::Update(double dt)
{
	GameObject::Update(dt);

	if (Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->GetGOComponent<Inven>()->GetIsOpened())
	{
		int row = 1;
		int column = 1;

		for (auto& info : shop_infos)
		{
			Engine::GetIconManager().AddIcon(
				info.icon,
				{ -575.0f + (120.0f * column), 200.0f - (120.0f * row) },
				0.8f
			);

			if (column == 3)
			{
				column = 0;
				row++;
			}
			column++;
		}

	}
}

void Shop::Draw(DrawLayer drawlayer)
{
	if(Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->GetGOComponent<Inven>()->GetIsOpened())
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