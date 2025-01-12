#include "Shop.h"

static bool is_on_inven = false;
static bool is_on_shop = false;
static bool Ready_to_buy = false;
static bool Ready_to_sell = false;

Shop::Shop():draw_call((GLTexture*)nullptr,nullptr, nullptr)
{
	skill_ptr = static_cast<Skillsys*>(Engine::Instance().GetTmpPtr());
	shop_background = Engine::GetTextureManager().Load("assets/images/temp_box1.png");
	shop_button = Engine::GetTextureManager().Load("assets/images/temp_box3.png");
	shop_icon = Engine::GetTextureManager().Load("assets/images/temp_box2.png");
	inven_background = Engine::GetTextureManager().Load("assets/images/temp_box1.png");
	base_icon_direction = { static_cast < float>(-shop_background->GetWidth()), static_cast<float>(Engine::window_height - (inven_background->GetHeight() * 0.9)) };
	defualt_icon_direction = { static_cast<float>(-shop_background->GetWidth()), static_cast<float>(Engine::window_height - (inven_background->GetHeight() * 0.9)) }; // 아이콘 기본값
	inven_back_pos = { 300, static_cast<float>(Engine::window_height - (inven_background->GetHeight() * 1.3)) };
	botton_pos = { static_cast<float>(-shop_background->GetWidth() + shop_button->GetWidth()), static_cast<float>(Engine::window_height - (inven_background->GetHeight() * 0.9)) };
	back_matrix_defualt = { static_cast<float>(100 - shop_background->GetWidth()), static_cast<float>(Engine::window_height - (inven_background->GetHeight() * 1.3)) };
	for (int i = 1; i < 4; i++)
	{
		icon_matrix.push_back(mat3::build_translation({ base_icon_direction.x , base_icon_direction.y - (float) (i*(shop_background->GetHeight()/4)) }) * mat3::build_scale(0.4f));
		inv_icon_matrix.push_back(mat3::build_translation({ base_icon_direction.x + shop_background->GetHeight()*1.1f , base_icon_direction.y - (float)(i * (shop_background->GetHeight() / 4)) }) * mat3::build_scale(0.4f));
	}
	icon_draw_calls.resize(3);
	//Read save file?
}


void Shop::Update(double dt)
{
	if (Engine::GetInput().KeyJustPressed(Input::Keys::Y) && !shop_available)
	{
		shop_available = true;
		std::cout << "Shop active" << std::endl;
	}

	if (shop_available) // Is shop available?
	{
		Shop_Back_draw(); // draw shop background
		Inventory_Back_draw();
		Shop_icon_draw();
		Shop_button_draw();
		Inventory_Icon_draw();

		if (pick == First)
		{
			if (Engine::Instance().GetInput().KeyJustPressed(Input::Keys::Enter) || Ready_to_buy)
			{
				Buy(skill_ptr->Net, Net_Money);
				Ready_to_buy = false;
			}
		}

		if (pick == Second)
		{
			if (Engine::Instance().GetInput().KeyJustPressed(Input::Keys::Enter) || Ready_to_buy)
			{
				Buy(skill_ptr->Light, Net_Money);
				Ready_to_buy = false;
			}
		}

		if (pick == Third)
		{
			if (Engine::Instance().GetInput().KeyJustPressed(Input::Keys::Enter) || Ready_to_buy)
			{
				Buy(skill_ptr->GARRY, Net_Money);
				Ready_to_buy = false;
			}
		}

	}
}

void Shop::Buy(Skillsys::Skill_list skill,int input)
{
	skill_ptr->AddSkill(skill, input);
}

void Shop::Sell(Skillsys::Skill_list skill, int input)
{
	skill_ptr->RemoveSkill(skill, input);
}

void Shop::Shop_Back_draw()
{

}

void Shop::Shop_icon_draw()
{

}





void Shop::Shop_button_draw()
{

}

void Shop::Inventory_Back_draw()
{

}

void Shop::Inventory_Icon_draw()
{

}

