#include "Shop.h"

static bool is_on_inven = false;
static bool is_on_shop = false;
static bool Ready_to_buy = false;
static bool Ready_to_sell = false;

Shop::Shop() : GameObject(back_position_default)
{
	skill_ptr = static_cast<Skillsys*>(Engine::Instance().GetTmpPtr());


	shop_background = Engine::GetTextureManager().Load("assets/images/temp_box1.png");
	shop_button = Engine::GetTextureManager().Load("assets/images/temp_box3.png");
	shop_icon = Engine::GetTextureManager().Load("assets/images/temp_box2.png");
	inven_background = Engine::GetTextureManager().Load("assets/images/temp_box1.png");


	base_icon_direction = { static_cast<float>(-shop_background->GetWidth()), static_cast<float>(Engine::window_height - (inven_background->GetHeight() * 0.9)) }; // ������ �⺻��
	inven_back_pos = { 300, static_cast<float>(Engine::window_height - (inven_background->GetHeight() * 1.3)) };
	botton_pos = { static_cast<float>(-shop_background->GetWidth() + shop_button->GetWidth()), static_cast<float>(Engine::window_height - (inven_background->GetHeight() * 0.9)) };
	back_position_default = { static_cast<float>(100 - shop_background->GetWidth()), static_cast<float>(Engine::window_height - (inven_background->GetHeight() * 1.3)) };

	// Setup for icon and add it to IconManager, will be update more :(

	Icon* test_icon_ptr = new Icon({ base_icon_direction.x , base_icon_direction.y - (float)((shop_background->GetHeight() / 4)) }, shop_icon);
	Icon* test_icon_ptr2 = new Icon({ base_icon_direction.x , base_icon_direction.y - (float)((shop_background->GetHeight() / 4) * 2) }, shop_icon);
	icon_manager_ptr = new IconManager();
	icon_manager_ptr->Add(test_icon_ptr);
	icon_manager_ptr->Add(test_icon_ptr2);
}

Shop::~Shop()
{
	delete icon_manager_ptr;
}


void Shop::Update(double dt)
{
	//if (Engine::GetInput().KeyJustPressed(Input::Keys::Y) && !shop_available)
	//{
	//	shop_available = true;
	//	std::cout << "Shop active" << std::endl;
	//}

	//if (shop_available) // Is shop available?
	//{
	//	Shop_Back_draw(); // draw shop background
	//	Inventory_Back_draw();
	//	icon_manager_ptr->UpdateAll(dt);
	//	icon_manager_ptr->DrawAll();

	//	if (pick == First)
	//	{
	//		if (Engine::Instance().GetInput().KeyJustPressed(Input::Keys::Enter) || Ready_to_buy) 
	//		/*
	//		*  I'll change this if statements later, I implemented the function to buy with Enter and the function to buy by dragging, and this is what happened!
	//		*/
	//		{
	//			Buy(skill_ptr->Net, Net_Money);
	//			Ready_to_buy = false;
	//		}
	//	}

	//	if (pick == Second)
	//	{
	//		if (Engine::Instance().GetInput().KeyJustPressed(Input::Keys::Enter) || Ready_to_buy)
	//		{
	//			Buy(skill_ptr->Light, Net_Money);
	//			Ready_to_buy = false;
	//		}
	//	}

	//	if (pick == Third)
	//	{
	//		if (Engine::Instance().GetInput().KeyJustPressed(Input::Keys::Enter) || Ready_to_buy)
	//		{
	//			Buy(skill_ptr->TEMP1, Net_Money);
	//			Ready_to_buy = false;
	//		}
	//	}

	//}
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

	//back_matrix = mat3::build_translation({ back_position_default }) * mat3::build_scale(1.0f); //* mat3::build_rotation(3.141592f/2.0f);

	//draw_call = 
	//{
	//	shop_background,                       // Texture to draw
	//	&back_matrix,                          // Transformation matrix
	//	Engine::GetShaderManager().GetDefaultShader(), // Shader to use
	//	nullptr,
	//	settings
	//};

	//Engine::GetRender().AddDrawCall(draw_call, DrawLayer::DrawFirst);
}


