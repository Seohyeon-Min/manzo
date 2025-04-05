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

	int GetMoney() { return money; }
	bool GetFishState() { return current_state == &state_fc;	}
	float GetX1Pos() { return m1x; }
	float GetX2Pos() { return m2x; }

	int HowManyFishes(int index) { return fishCollection[index]; }
	int HowMuchSold() { return how_much_sold; }


	std::map<int, int> fishCollection;

private:
	bool is_opened = false;
	bool is_picked = false;
	bool has_sold = false;
	bool in_fish_state = false;
	int page = 0;
	int money = 0;
	int total_fishNum = 3;
	int todays_fish_index = 0;
	int todays_price = 0;
	int how_much_sold = 0;
	float m1x, m2x;

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

	std::vector<vec2> savePos = {
		{ -130, 100 },
		{ 0, 100 },
		{ 130, 100 }
	};

};