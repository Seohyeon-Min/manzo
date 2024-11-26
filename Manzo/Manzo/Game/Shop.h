#pragma once
#include "..\Engine/Engine.h"
#include <string>
#include <vector>
#include <iostream>
#include "..\Game\Ship.h"
#include "../Engine/Component.h"
#include "Fish.h"
#include "Skill.h"

class Skillsys;

class Shop : public CS230::Component
{
public:

	enum shop_list
	{
		First = 1,
		Second,
		Third,
	};
	Shop();
	void Update(double dt);
	void Buy(Skillsys::Skill_list skill, int input);
	void Shop_Back_draw();
	void Shop_icon_draw();
	void Shop_button_draw();


private:

	Skillsys* skill_ptr;
	int Net_Money = 3;
	bool shop_available = false;
	GLTexture* shop_background = nullptr;
	GLTexture* shop_icon = nullptr;
	GLTexture* shop_button = nullptr;
	shop_list pick = First;
	mat3 back_matrix;
	//mat3 icon_matrix;
	mat3 botton_matrix;
	CS230::DrawCall draw_call;
	std::vector<mat3> icon_matrix;
	std::vector<CS230::DrawCall> icon_draw_calls;

};