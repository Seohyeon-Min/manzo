#pragma once

#include "../Engine/GameObject.h"

#include <random>
#include "Module.h"

class Inven : public GameObject
{
public:
	Inven(vec2 position);
	GameObjectTypes Type() override { return GameObjectTypes::UI; }
	std::string TypeName() override { return "Inventory UI"; }

	void Update(double dt) override;
	void Draw(DrawLayer drawlayer = DrawLayer::Draw) override;

	bool GetIsOpened() { return is_opened; }
	void SetIsOpened(bool open) { is_opened = open; }
	bool Open();

	void BuyFirstModule(bool buy) { buy_first_module = buy; }
	void BuySecondModule(bool buy) { buy_second_module = buy; }

	bool FirstModuleBought() { return buy_first_module; }
	bool SecondModuleBought() { return buy_second_module; }

	void SetMoney(int new_money) { money = new_money; }
	int GetMoney() { return money; }

	bool GetFishState() { return current_state == &state_fc;	}
	float GetX1Pos() { return m1x; }
	float GetX2Pos() { return m2x; }

	int HowManyFishes(int index) { return fishCollection[index]; }
	int TodayFishPrice() { return todays_price; }
	int HowMuchSold() { return how_much_sold; }

	void SetHowMuchSold(int new_value) { how_much_sold = new_value; }
	int GetTodayFishIndex() { return todays_fish_index; }

	std::map<int, int> fishCollection;
	std::map<int, int> originCollection;

	std::vector<vec2> savePos = {
		{ -65, 100 },
		{ 65, 100 }
	};

	int which_fish_sellected = 0;

private:
	bool is_opened = false;
	bool is_picked = false;
	bool has_sold = false;

	bool in_fish_state = false;

	bool buy_first_module;
	bool buy_second_module;

	int page = 0;
	int money = 0;

	int total_fishNum = 7;
	int todays_fish_index = 0;
	int todays_price = 0;
	int how_much_sold = 1;


	float m1x, m2x;
	int p = 200;

	bool holding = false;
	std::string todays_fish_icon;

	std::mt19937 dre_todayFish;
	std::mt19937 dre_price;

	enum class Animations {
		Module,
		FishCollection,
		SpecialCollection
	};

	class State_None : public State {
	public:
		virtual void Enter(GameObject* object) override;
		virtual void Update(GameObject* object, double dt) override;
		virtual void CheckExit(GameObject* object) override;
		std::string GetName() override { return "Inventory - None"; }
	};

	class State_Module : public State {
	public:
		virtual void Enter(GameObject* object) override;
		virtual void Update(GameObject* object, double dt) override;
		virtual void CheckExit(GameObject* object) override;
		std::string GetName() override { return "Inventory - Module"; }
	};

	class State_FC : public State {
	public:
		virtual void Enter(GameObject* object) override;
		virtual void Update(GameObject* object, double dt) override;
		virtual void CheckExit(GameObject* object) override;
		std::string GetName() override { return "Inventory - Fish Collection"; }
	};

	class State_SC : public State {
	public:
		virtual void Enter(GameObject* object) override;
		virtual void Update(GameObject* object, double dt) override;
		virtual void CheckExit(GameObject* object) override;
		std::string GetName() override { return "Inventory - Special Collection"; }
	};

	State_None state_none;
	State_Module state_module;
	State_FC state_fc;
	State_SC state_sc;

	Module* module_ptr;

	friend class Shop;
};