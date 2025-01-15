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

/*
	�ؾ��Ұ�

	1. �ܼ� Set �Լ� ������Ϸ� �̵�
	2. for�� ��� STL ����
	3. update �� å�� �и� �Ǵ°� �и��ؼ� ���� �Լ��� �����.
		�� SelectSlot(), SelectSkill(), HandleSlotSelection(), ApplySkillToSlot() ������ ����ٴ���..
		�� Update�� ���1, ���2�� ���� ���� ���1�� ������Ʈ �Լ�, ���2�� ������Ʈ �Լ� �����.
*/ 

class Skillsys //skill system
{

public:
	enum Skill_list
	{
		Empty,
		Net,
		Light,

		TEMP1, // after this, all enum is just temp name.
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

	void					Update();
	void					SetSkill(int slot, Skill_list skill) { skillslots[slot] = skill; };
	void					SkillPrint(); // for debug, print skillslot info
	void					ClearSkill(); // Clear skillslot vector
	void					SetShipPtr(Ship* ptr); // need ship..
	Ship*					GetShipPtr(); 
	std::vector<Skill_list> GetSkill() { return skillslots; };
	void					AddSkill(Skill_list input, int money);
	void					RemoveSkill(Skill_list input, int money);
	Skill_list				ChangeNumberToList(int input) { return static_cast<Skill_list>(input); };
	std::vector<Skill_list> GetInventory() { return inventory; };

	void					Active_skill(Skill_list skill);
	void					SkillNet();
	void					SkillLight();
	void					PrintInven();

	//Class for each skill

	class Skill_Net : public GameObject // Net
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
	std::vector<Skill_list>			skillslots;
	std::vector<Skill_list>			inventory;
	bool							is_slot_selected = false;
	bool							ready_to_set = false;
	bool							is_active_skill = false;
	int								selected_slot = NULL;
	bool							check_ship_ptr = false;
	Skill_list						selected_skill = Empty;
	Ship*							ship_ptr = nullptr;
	Skill_Net*						skill_net;

	//For Skill


};