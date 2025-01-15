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
	if (Engine::GetInput().KeyJustPressed(Input::Keys::Y) && !shop_available)
	{
		shop_available = true;
		std::cout << "Shop active" << std::endl;
	}

	if (shop_available) // Is shop available?
	{
		Shop_Back_draw(); // draw shop background
		Inventory_Back_draw();
		icon_manager_ptr->UpdateAll(dt);
		icon_manager_ptr->DrawAll();

		if (pick == First)
		{
			if (Engine::Instance().GetInput().KeyJustPressed(Input::Keys::Enter) || Ready_to_buy) 
			/*
			*  I'll change this if statements later, I implemented the function to buy with Enter and the function to buy by dragging, and this is what happened!
			*/
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
				Buy(skill_ptr->TEMP1, Net_Money);
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

	back_matrix = mat3::build_translation({ back_position_default }) * mat3::build_scale(1.0f); //* mat3::build_rotation(3.141592f/2.0f);

	draw_call = 
	{
		shop_background,                       // Texture to draw
		&back_matrix,                          // Transformation matrix
		Engine::GetShaderManager().GetDefaultShader(), // Shader to use
		nullptr,
		settings
	};

	Engine::GetRender().AddDrawCall(draw_call, DrawLayer::DrawFirst);
}

/* void Shop::Shop_icon_draw()
{

	static int dragging_icon_index = -1;   // �巡�� ���� �������� �ε���
	static bool is_dragging = false;       // �巡�� ���� Ȯ��
	static float drag_start_pos_x = 0.0f;  // �巡�� ���� ������ ���콺 X ��ǥ
	static float drag_start_pos_y = 0.0f;  // �巡�� ���� ������ ���콺 Y ��ǥ
	

	// �� �������� ������ (�����ܺ��� ����)
	static std::vector<vec2> icon_positions = {
		{ base_icon_direction.x, base_icon_direction.y - (shop_background->GetHeight() / 4) },
		{ base_icon_direction.x, base_icon_direction.y - (shop_background->GetHeight() / 4 * 2) },
		{ base_icon_direction.x, base_icon_direction.y - (shop_background->GetHeight() / 4 * 3) }
	};

	float current_mouse_pos_x = Engine::Instance().GetInput().GetMousePos().mouseCamSpaceX;
	float current_mouse_pos_y = Engine::Instance().GetInput().GetMousePos().mouseCamSpaceY;
	vec2 current_mouse_pos = { Engine::Instance().GetInput().GetMousePos().mouseCamSpaceX, Engine::Instance().GetInput().GetMousePos().mouseCamSpaceY };

	if (!is_dragging) {
		// �巡�� ���� �ƴϸ� ������ Ŭ�� ����
		for (int i = 0; i < icon_positions.size(); i++) {
			vec2 icon_pos = icon_positions[i];
			if ((current_mouse_pos_x > icon_pos.x - (shop_icon->GetWidth() / 4) &&
				current_mouse_pos_x < icon_pos.x + (shop_icon->GetWidth() / 4)) &&
				(current_mouse_pos_y > icon_pos.y - (shop_icon->GetHeight() / 4) &&
					current_mouse_pos_y < icon_pos.y + (shop_icon->GetHeight() / 4)))
			{
				if (Engine::Instance().GetInput().MouseButtonJustPressed(1)) {
					std::cout << "You clicked the " << i + 1 << "th icon!" << std::endl;
					dragging_icon_index = i;
					is_dragging = true;
					pick = (shop_list)(i + 1);

					// �巡�� ���� ��ġ ����
					drag_start_pos_x = current_mouse_pos_x;
					drag_start_pos_y = current_mouse_pos_y;
					break;
				}
			}
		}
	}
	else {
		// �巡�� ���� ���
		if (Engine::Instance().GetInput().MouseButtonJustReleased(1)) {
			// ���콺 ��ư�� ������ �巡�� ����
			std::cout << "Released" << std::endl;
			is_dragging = false;

			// �巡�� ���� �� �巡���� �������� ������ ����(�ϴ� �ʱ� ��ġ��)
			if (!is_on_inven)
			{
				icon_positions[dragging_icon_index].x = base_icon_direction.x;
				icon_positions[dragging_icon_index].y = base_icon_direction.y - ((dragging_icon_index + 1) * (shop_background->GetHeight() / 4));
			}
			if (is_on_inven)
			{
				Ready_to_buy = true;
				is_on_inven = false;
				inv_info[dragging_icon_index].icon_texture = shop_icon;
				inv_info[dragging_icon_index].skill = skill_ptr->ChangeNumberToList(dragging_icon_index + 1);
			}

			dragging_icon_index = -1;

		}
		else { // �巡�� ���϶��� ����
			// ���콺 �̵��� ���� �巡�� ���� ������ ��ġ ���� (���� �̵��� ���)
			float delta_x = current_mouse_pos_x - drag_start_pos_x;
			float delta_y = current_mouse_pos_y - drag_start_pos_y;

			icon_matrix[dragging_icon_index] =
				mat3::build_translation(icon_positions[dragging_icon_index].x + delta_x,
					icon_positions[dragging_icon_index].y + delta_y) *
				mat3::build_scale(0.1f);
			if( (icon_positions[dragging_icon_index].x + delta_x >= inven_back_pos.x - (shop_background->GetWidth()/4) && icon_positions[dragging_icon_index].x + delta_x <= inven_back_pos.x + (shop_background->GetWidth()/4))
				&& (icon_positions[dragging_icon_index].y + delta_y >= inven_back_pos.y - (shop_background->GetHeight()/2)&& icon_positions[dragging_icon_index].y + delta_y <= inven_back_pos.y + (shop_background->GetHeight()/2)))
			{
				is_on_inven = true;
			}
		}
	}

	// ������ �����
	for (int i = 0; i < icon_positions.size(); i++) {
		if (i != dragging_icon_index || !is_dragging) {
			// �巡�� ���� �������� �ǽð����� ������Ʈ�ǹǷ� �����ϰ� �׸� �غ�
			icon_matrix[i] = mat3::build_translation(icon_positions[i]) * mat3::build_scale(0.4f);
		}

		icon_draw_calls[i] = {
			shop_icon,                                 // Texture to draw
			&icon_matrix[i],                           // Transformation matrix
			Engine::GetShaderManager().GetDefaultShader(), // Shader to use
			nullptr,
			settings
		};
	}

	Engine::GetRender().AddDrawCall(icon_draw_calls[0], DrawLayer::DrawFirst);
	Engine::GetRender().AddDrawCall(icon_draw_calls[1], DrawLayer::DrawFirst);
	Engine::GetRender().AddDrawCall(icon_draw_calls[2], DrawLayer::DrawFirst);
}


void Shop::Shop_button_draw()
{

}

void Shop::Inventory_Back_draw()
{

}

void Shop::Inventory_Icon_draw()
{

	static int dragging_icon_index = -1;   // �巡�� ���� �������� �ε���
	static bool is_dragging = false;       // �巡�� ���� Ȯ��
	static float drag_start_pos_x = 0.0f;  // �巡�� ���� ������ ���콺 X ��ǥ
	static float drag_start_pos_y = 0.0f;  // �巡�� ���� ������ ���콺 Y ��ǥ

	inv_info.resize(3);

	static std::vector<vec2> icon_positions = {
	{ base_icon_direction.x + shop_background->GetHeight()*1.1f, base_icon_direction.y - (shop_background->GetHeight() / 4) },
	{ base_icon_direction.x + shop_background->GetHeight()*1.1f, base_icon_direction.y - (shop_background->GetHeight() / 4 * 2) },
	{ base_icon_direction.x + shop_background->GetHeight()*1.1f, base_icon_direction.y - (shop_background->GetHeight() / 4 * 3) }
	};
	
	float current_mouse_pos_x = Engine::Instance().GetInput().GetMousePos().mouseCamSpaceX;
	float current_mouse_pos_y = Engine::Instance().GetInput().GetMousePos().mouseCamSpaceY;

	if (!is_dragging) {
		// �巡�� ���� �ƴϸ� ������ Ŭ�� ����
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

					// �巡�� ���� ��ġ ����
					drag_start_pos_x = current_mouse_pos_x;
					drag_start_pos_y = current_mouse_pos_y;
					break;
				}
			}
		}
	}
	else {
		// �巡�� ���� ���
		if (Engine::Instance().GetInput().MouseButtonJustReleased(1)) {
			// ���콺 ��ư�� ������ �巡�� ����
			std::cout << "Released" << std::endl;
			is_dragging = false;

			// �巡�� ���� �� �巡���� �������� ������ ����(�ϴ� �ʱ� ��ġ��)
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
		else { // �巡�� ���϶��� ����
			// ���콺 �̵��� ���� �巡�� ���� ������ ��ġ ���� (���� �̵��� ���)
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
				// �巡�� ���� �������� �ǽð����� ������Ʈ�ǹǷ� �����ϰ� �׸� �غ�
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

