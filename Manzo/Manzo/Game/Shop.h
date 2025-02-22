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
#include "icon.h"
#include "IconManager.h"

// class Skillsys;

/*
	�ؾ��Ұ�

	1. ������Ʈ�� ��������� �Ǵ��� Ȯ�� // ok
	2. Shop ���� ����ȭ // ������...
	3. ������ �˾ƺ��� ���� // ������..
	4. Icon Ŭ����(������Ʈ) �� �и� // ũ�ƾƾ�
	5. Buy ���� ����ȭ // �׳� Ready_to_buy�� �ϴ°�?
	6. shop_list�� �������� ���ٳ� �迭����
	7. money ���õȰ� shop����, skill���� remove��.
	8. x,y ��ǥ �ٷ�°� vec2�� �ٷ��.
	
*/

enum Skillsys::Skill_list;

class Shop : public GameObject
{
public:

	enum shop_list /** For player's pick*/
	{
		First = 1,
		Second,
		Third,
	};

	struct Inventory_info // each Inventory's info
	{
		Skillsys::Skill_list skill; //set Empty
		GLTexture* icon_texture = nullptr;
	};

	Shop();
	~Shop();
	GameObjectTypes			Type() override { return GameObjectTypes::Shop; };
	std::string				TypeName() override { return "Shop"; };
	void					Update(double dt); // maybe not used dt..
	void					Buy(Skillsys::Skill_list skill, int input); //Skill = which player's buy? input = How much use money for that skill?
	void					Sell(Skillsys::Skill_list skill, int input);
	void					Shop_Back_draw();
	void					Shop_button_draw();
	void					Inventory_Back_draw();


private:

	Skillsys*					skill_ptr;
	//Icon*						icon_ptr;
	IconManager*				icon_manager_ptr;
	int							Net_Money = 3;
	bool						shop_available = false; // false = shop disappear / true = shop appear
	GLTexture*					shop_background = nullptr; //texture
	GLTexture*					shop_icon = nullptr; //texture
	GLTexture*					shop_button = nullptr; //texture
	GLTexture*					inven_background = nullptr; //texture
	shop_list					pick = First; //player's pick
	shop_list					sell_pick = First;
	mat3						back_matrix; // shop background matrix
	mat3						inv_back_matrix; // inventory background matrix
	mat3						botton_matrix; // shop botton matrix
	vec2						base_icon_direction;
	vec2						botton_pos;
	vec2						inven_back_pos;
	vec2						shop_back_pos;
	vec2						back_position_default;
	DrawCall					draw_call;
	std::vector<mat3>			icon_matrix;
	std::vector<mat3>			inv_icon_matrix;
	std::vector<DrawCall>		icon_draw_calls;
	std::vector<DrawCall>		inv_icon_draw_calls; //for inventory icon
	std::vector<vec2>			icon_direction;
	std::vector<Inventory_info>	inv_info;

};   

/*
		struct�� ���� ���� ���°͵� ��� Ÿ������ ���´ٴ��� �ƴ� icon Ŭ���� ���� ����°ŷ� ������ �ɵ�?

*/