#pragma once
#include "..\Engine/Engine.h"
#include <string>
#include <vector>
#include <iostream>
#include "..\Game\Ship.h"
#include "../Engine/GameObject.h"
#include "../Engine/GameObjectManager.h"
#include "..\Engine\Engine.h"
#include "Fish.h"
#include "Skill.h"
#include "Inventory.h"
#include "..\Engine\Sprite.h"

class Shop : public GameObject
{
public:

	enum shop_list /** For player's pick*/
	{
		First = 1,
		Second,
		Third,
	};

	struct shop_info
	{
		std::string name = "None";
		std::string icon = "None";
		int price = 0;
		std::string script = "Defualt script";
	};

	Shop(vec2 postion);
	~Shop();
	GameObjectTypes			Type() override { return GameObjectTypes::Shop; };
	std::string				TypeName() override { return "Shop"; };
	void					Update(double dt);
	void					Draw(DrawLayer drawlayer = DrawLayer::Draw) override;
	void					Read_Shop_Csv(const std::string& filename);

	bool CanCollideWith(GameObjectTypes other_object) override;
	//void ResolveCollision(GameObject* other_object) override;

private:

	Inven* inven;
	Skillsys*					skill_ptr;
	int							Net_Money = 3;
	bool						shop_available = false; // false = shop disappear / true = shop appear
	bool already_buy;
	
	shop_list					pick = First; //player's pick
	shop_list					sell_pick = First;
	std::vector<shop_info>		shop_infos;
};   