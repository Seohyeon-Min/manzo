#include "Shop.h"
#include "../Engine/Dragging.h"

static bool is_on_inven = false;
static bool is_on_shop = false;
static bool Ready_to_buy = false;
static bool Ready_to_sell = false;

Shop::Shop() : GameObject(back_position_default)
{
}

Shop::~Shop()
{
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

void Shop::Shop_button_draw()
{
	botton_matrix = mat3::build_translation({ botton_pos.x , botton_pos.y - (float)(pick * (shop_background->GetHeight()/4)) }) * mat3::build_scale(1.0f);

	if (Engine::Instance().GetInput().KeyJustPressed(Input::Keys::Up))
	{
			switch (pick)
			{
			case Shop::First:
				pick = Third;
				break;
			case Shop::Second:
				pick = First;
				break;
			case Shop::Third:
				pick = Second;
				break;
			}
	}

	if (Engine::Instance().GetInput().KeyJustPressed(Input::Keys::Down))
	{
			switch (pick)
			{
			case Shop::First:
				pick = Second;
				break;
			case Shop::Second:
				pick = Third;
				break;
			case Shop::Third:
				pick = First;
				break;
			}
	}

	DrawCall draw_call =
	{
		shop_button,                       // Texture to draw
		&botton_matrix,                          // Transformation matrix
		Engine::GetShaderManager().GetDefaultShader(), // Shader to use
	};

	draw_call.settings.is_camera_fixed = true;
	draw_call.settings.do_blending = true;
	draw_call.sorting_layer = DrawLayer::DrawFirst;

	Engine::GetRender().AddDrawCall(std::make_unique<DrawCall>(draw_call));
} 

void Shop::Inventory_Back_draw()
{
	inv_back_matrix = mat3::build_translation({ inven_back_pos }) * mat3::build_scale(1.0f); // * mat3::build_rotation(3.141592f / 2.0f);

	DrawCall draw_call =
	{
		inven_background,                       // Texture to draw
		&inv_back_matrix,                          // Transformation matrix
		Engine::GetShaderManager().GetDefaultShader(), // Shader to use
	};

	draw_call.settings.is_camera_fixed = true;
	draw_call.settings.do_blending = true;
	draw_call.sorting_layer = DrawLayer::DrawFirst;

	Engine::GetRender().AddDrawCall(std::make_unique<DrawCall>(draw_call));
}
/*
void Shop::Inventory_Icon_draw()
{
	DrawSettings settings;
	settings.is_camera_fixed = true;
	settings.do_blending = true;

	static int dragging_icon_index = -1; 
	static bool is_dragging = false;
	static float drag_start_pos_x = 0.0f; 
	static float drag_start_pos_y = 0.0f;  

	inv_info.resize(3);

	static std::vector<vec2> icon_positions = {
	{ base_icon_direction.x + shop_background->GetHeight()*1.1f, base_icon_direction.y - (shop_background->GetHeight() / 4) },
	{ base_icon_direction.x + shop_background->GetHeight()*1.1f, base_icon_direction.y - (shop_background->GetHeight() / 4 * 2) },
	{ base_icon_direction.x + shop_background->GetHeight()*1.1f, base_icon_direction.y - (shop_background->GetHeight() / 4 * 3) }
	};
	
	float current_mouse_pos_x = Engine::Instance().GetInput().GetMousePos().mouseCamSpaceX;
	float current_mouse_pos_y = Engine::Instance().GetInput().GetMousePos().mouseCamSpaceY;

	if (!is_dragging) {

		for (int i = 0; i < icon_positions.size(); i++) {
			vec2 icon_pos = icon_positions[i];
			if ((current_mouse_pos_x > icon_pos.x - (shop_icon->GetWidth() / 4) &&
				current_mouse_pos_x < icon_pos.x + (shop_icon->GetWidth() / 4)) &&
				(current_mouse_pos_y > icon_pos.y - (shop_icon->GetHeight() / 4) &&
					current_mouse_pos_y < icon_pos.y + (shop_icon->GetHeight() / 4)))
			{
				if (Engine::Instance().GetInput().MouseButtonJustPressed(1) && inv_info[i].icon_texture != nullptr) {
					std::cout << "You clicked the " << i + 1 << "th icon!" << std::endl;
					dragging_icon_index = i;
					is_dragging = true;

					drag_start_pos_x = current_mouse_pos_x;
					drag_start_pos_y = current_mouse_pos_y;
					break;
				}
			}
		}
	}
	else {

		if (Engine::Instance().GetInput().MouseButtonJustReleased(1)) {

			std::cout << "Released" << std::endl;
			is_dragging = false;

			if (!is_on_shop)
			{
				icon_positions[dragging_icon_index].x = base_icon_direction.x + shop_background->GetHeight()*1.1f;
				icon_positions[dragging_icon_index].y = base_icon_direction.y - ((dragging_icon_index + 1) * (shop_background->GetHeight() / 4));
			}
			if (is_on_shop)
			{
				Ready_to_sell = true;
				//std::cout << "is on the shop" << std::endl;
				inv_info[dragging_icon_index].icon_texture = nullptr;
				is_on_shop = false;
			}
			if (Ready_to_sell)
			{
				Sell(inv_info[dragging_icon_index].skill, Net_Money);
				Ready_to_sell = false;
			}

			dragging_icon_index = -1;

		}
		else { 
			
			float delta_x = current_mouse_pos_x - drag_start_pos_x;
			float delta_y = current_mouse_pos_y - drag_start_pos_y;

			inv_icon_matrix[dragging_icon_index] =
				mat3::build_translation(icon_positions[dragging_icon_index].x + delta_x,
					icon_positions[dragging_icon_index].y + delta_y) *
				mat3::build_scale(0.4f);
			if ((icon_positions[dragging_icon_index].x + delta_x >= shop_back_pos.x - (shop_background->GetWidth() / 4) && icon_positions[dragging_icon_index].x + delta_x <= shop_back_pos.x + (shop_background->GetWidth() / 4))
				&& (icon_positions[dragging_icon_index].y + delta_y >= shop_back_pos.y - (shop_background->GetHeight() / 2) && icon_positions[dragging_icon_index].y + delta_y <= shop_back_pos.y + (shop_background->GetHeight() / 2)))
			{
				is_on_shop = true;
			}
		}
	}

	if (skill_ptr->GetInventory().size() >= 1)
	{
		for (int i = 0; i < icon_positions.size(); i++) {
			if (i != dragging_icon_index || !is_dragging) {
				
				inv_icon_matrix[i] = mat3::build_translation(icon_positions[i]) * mat3::build_scale(0.4f);
			}
			if (inv_info[i].icon_texture != nullptr)
			{
				icon_draw_calls[i] = {
					inv_info[i].icon_texture,                                 // Texture to draw
					&inv_icon_matrix[i],                           // Transformation matrix
					Engine::GetShaderManager().GetDefaultShader(), // Shader to use
					nullptr,
					settings
				};
				Engine::GetRender().AddDrawCall(icon_draw_calls[i], DrawLayer::DrawFirst);
			}
		}
	}
} */

