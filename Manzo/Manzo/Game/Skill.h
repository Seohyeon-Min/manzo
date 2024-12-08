#pragma once
#include "..\Engine/Engine.h"
#include <string>
#include <vector>
#include <iostream>
#include "..\Game\Ship.h"
#include "../Engine/Component.h"
#include "..\Engine\GameObjectManager.h"
#include "Fish.h"

class Ship;

class Fish;

class Skillsys //skill system
{

public:
	enum Skill_list
	{
		Empty,
		Net,
		Light,

		GARRY,
		GILL,
		GLEN,
		IDONTKNOW,
		SOMETHING_WITH_A_G
	};

	enum state
	{
		Set,
		Active,
	};

	Skillsys() : skillslots(3, Empty), inventory(0) { is_active_skill = false; }

	void Update();
	void setskill(int slot, Skill_list skill);
	void skillprint(); // for debug, print skillslot info
	void ClearSkill(); // Clear skillslot vector
	void SetShipPtr(Ship* ptr); // need ship..
	Ship* GetShipPtr(); 
	std::vector<Skill_list> GetSkill() { return skillslots; };
	void AddSkill(Skill_list input, int money);
	std::vector<Skill_list> GetInventory() { return inventory; };

	void Active_skill(Skill_list skill);
	void SkillNet();
	void SkillLight();
	void PrintInven();

	//Class for each skill

	class Skill_Net : public CS230::GameObject // Net
	{

	public:

		Skill_Net(vec2 position, Skillsys* skillsys);
		GameObjectTypes Type() override { return GameObjectTypes::Net; }
		std::string TypeName() override { return "Net"; }
		bool CanCollideWith(GameObjectTypes) override;
		//void ResolveCollision([[maybe_unused]] GameObject* other_object) override;
		void SetShipPtr(Ship* ptr) { ship_ptr = ptr; }
		void Update(double [[maybe_unused]] dt);

		const vec2& GetPosition() const { return GameObject::GetPosition(); }

	private:

		void Draw();
		Ship* ship_ptr = nullptr;

	};

private:
	std::vector<Skill_list> skillslots;
	std::vector<Skill_list> inventory;
	bool is_slot_selected = false;
	bool Ready_to_set = false;
	bool is_active_skill = false;
	int Selected_slot = NULL;
	bool Check_ship_ptr = false;
	Skill_list Selected_skill = Empty;
	Ship* Ship_ptr = nullptr;
	Skill_Net* skill_net;

	//For Skill


};