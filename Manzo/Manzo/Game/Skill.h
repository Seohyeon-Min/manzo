#pragma once
#include "..\Engine/Engine.h"
#include <string>
#include <vector>
#include <iostream>
#include "..\Game\Ship.h"
#include "../Engine/Component.h"


class Skillsys : public CS230::Component //skill system
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

	Skillsys() : skillslots(3, Empty) {}

	void Update([[maybe_unused]]double dt);
	void setskill(int slot, Skill_list skill);
	void skillprint();
	void ClearSkill();

	void Active_skill(Skill_list skill);
	void Skill_Net();
	void Skill_Light();

private:
	std::vector<Skill_list> skillslots;
	bool is_slot_selected = false;
	bool Ready_to_set = false;
	int Selected_slot = NULL;
	Skill_list Selected_skill = Empty;


};
