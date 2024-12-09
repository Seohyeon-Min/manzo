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

	enum shop_list /** For player's pick*/
	{
		First = 1,
		Second,
		Third,
	};
	Shop();
	void Update(double dt); // maybe not used dt..
	void Buy(Skillsys::Skill_list skill, int input); //Skill = which player's buy? input = How much use money for that skill?
	void Sell(Skillsys::Skill_list skill, int input);
	void Shop_Back_draw();
	void Shop_icon_draw();
	void Shop_button_draw();
	void Inventory_Back_draw();
	void Inventory_Icon_draw();


private:

	Skillsys* skill_ptr;
	int Net_Money = 3;
	bool shop_available = false; // false = shop disappear / true = shop appear
	GLTexture* shop_background = nullptr; //texture
	GLTexture* shop_icon = nullptr; //texture
	GLTexture* shop_button = nullptr; //texture
	GLTexture* inven_background = nullptr; //texture
	shop_list pick = First; //player's pick
	shop_list sell_pick = First;
	mat3 back_matrix; // shop background matrix
	mat3 inv_back_matrix; // inventory background matrix
	mat3 botton_matrix; // shop botton matrix
	vec2 base_botton_direction;
	vec2 defualt_botton_direction;
	vec2 inven_back_pos;
	vec2 shop_back_pos;
	vec2 back_matrix_defualt;
	CS230::DrawCall draw_call;
	std::vector<mat3> icon_matrix;
	std::vector<mat3> inv_icon_matrix;
	std::vector<CS230::DrawCall> icon_draw_calls;
	std::vector<CS230::DrawCall> inv_icon_draw_calls; //for inventory icon
	std::vector<vec2> icon_direction;

};