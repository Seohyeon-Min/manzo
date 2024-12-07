#include "Shop.h"

static bool is_on_inven = false;
static bool Ready_to_buy = false;

Shop::Shop()
{
	skill_ptr = static_cast<Skillsys*>(Engine::Instance().GetTmpPtr());
	shop_background = Engine::GetTextureManager().Load("assets/images/temp_box1.png");
	shop_button = Engine::GetTextureManager().Load("assets/images/temp_box3.png");
	shop_icon = Engine::GetTextureManager().Load("assets/images/temp_box2.png");
	base_botton_direction = { 100, (Engine::window_height) / 2.2f };
	defualt_botton_direction = { 100, (Engine::window_height) / 2.2f }; // 아이콘 기본값
	inven_back_pos = { static_cast<float>(200 - shop_background->GetWidth() / 2), Engine::window_height / 13 };
	for (int i = 1; i < 4; i++)
	{
		icon_matrix.push_back(mat3::build_translation({ base_botton_direction.x , base_botton_direction.y - (float) (i*140) }) * mat3::build_scale(0.4f));
	}
	icon_draw_calls.resize(3);
	//Read save file?
}


void Shop::Update(double dt)
{
	if (Engine::GetInput().KeyJustPressed(CS230::Input::Keys::Y) && !shop_available)
	{
		shop_available = true;
		std::cout << "Shop active" << std::endl;
	}

	if (shop_available) // Can buy something
	{
		Shop_Back_draw(); // draw shop background
		Inventory_Back_draw();
		Shop_icon_draw();
		Shop_button_draw();
		Inventory_Icon_draw();

		if (pick == First)
		{
			if (Engine::Instance().GetInput().KeyJustPressed(CS230::Input::Keys::Enter) || Ready_to_buy)
			{
				Buy(skill_ptr->Net, Net_Money);
				Ready_to_buy = false;
			}
		}

		if (pick == Second)
		{
			if (Engine::Instance().GetInput().KeyJustPressed(CS230::Input::Keys::Enter) || Ready_to_buy)
			{
				Buy(skill_ptr->Light, Net_Money);
				Ready_to_buy = false;
			}
		}

		if (pick == Third)
		{
			if (Engine::Instance().GetInput().KeyJustPressed(CS230::Input::Keys::Enter) || Ready_to_buy)
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
	std::cout << "Shop inactive" << std::endl;
}

void Shop::Shop_Back_draw()
{
	DrawSettings settings;
	settings.is_UI = true;
	settings.do_blending = true;

	back_matrix = mat3::build_translation({ 200 , Engine::window_height / 13 }) * mat3::build_scale(0.7f) * mat3::build_rotation(3.141592f/2.0f);

	draw_call = 
	{
		shop_background,                       // Texture to draw
		&back_matrix,                          // Transformation matrix
		Engine::GetShaderManager().GetDefaultShader(), // Shader to use
		nullptr,
		settings
	};

	Engine::GetRender().AddDrawCall(draw_call, DrawLayer::DrawUI);
}

void Shop::Shop_icon_draw()
{
	DrawSettings settings;
	settings.is_UI = true;
	settings.do_blending = true;

	static int dragging_icon_index = -1;   // 드래그 중인 아이콘의 인덱스
	static bool is_dragging = false;       // 드래그 상태 확인
	static float drag_start_pos_x = 0.0f;  // 드래그 시작 시점의 마우스 X 좌표
	static float drag_start_pos_y = 0.0f;  // 드래그 시작 시점의 마우스 Y 좌표
	

	// 각 아이콘의 기준점 (아이콘별로 저장)
	static std::vector<vec2> icon_positions = {
		{ base_botton_direction.x, base_botton_direction.y - 140.0f },
		{ base_botton_direction.x, base_botton_direction.y - 280.0f },
		{ base_botton_direction.x, base_botton_direction.y - 420.0f }
	};

	float current_mouse_pos_x = Engine::Instance().GetInput().GetMousePos().mouseCamSpaceX;
	float current_mouse_pos_y = Engine::Instance().GetInput().GetMousePos().mouseCamSpaceY;

	if (!is_dragging) {
		// 드래그 중이 아니면 아이콘 클릭 감지
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

					// 드래그 시작 위치 저장
					drag_start_pos_x = current_mouse_pos_x;
					drag_start_pos_y = current_mouse_pos_y;
					break;
				}
			}
		}
	}
	else {
		// 드래그 중인 경우
		if (Engine::Instance().GetInput().MouseButtonJustReleased(1)) {
			// 마우스 버튼을 놓으면 드래그 종료
			std::cout << "Released" << std::endl;
			is_dragging = false;

			// 드래그 종료 후 드래그한 아이콘의 기준점 갱신(일단 초기 위치로)
			if (!is_on_inven)
			{
				icon_positions[dragging_icon_index].x = defualt_botton_direction.x;
				icon_positions[dragging_icon_index].y = defualt_botton_direction.y - ((dragging_icon_index + 1) * 140);
			}
			if (is_on_inven)
			{
				Ready_to_buy = true;
				is_on_inven = false;
			}

			dragging_icon_index = -1;

		}
		else { // 드래그 중일때의 로직
			// 마우스 이동에 따라 드래그 중인 아이콘 위치 갱신 (누적 이동량 기반)
			float delta_x = current_mouse_pos_x - drag_start_pos_x;
			float delta_y = current_mouse_pos_y - drag_start_pos_y;

			icon_matrix[dragging_icon_index] =
				mat3::build_translation(icon_positions[dragging_icon_index].x + delta_x,
					icon_positions[dragging_icon_index].y + delta_y) *
				mat3::build_scale(0.4f);
			if( (icon_positions[dragging_icon_index].x + delta_x >= inven_back_pos.x - (shop_background->GetHeight()/4) && icon_positions[dragging_icon_index].x + delta_x <= inven_back_pos.x + (shop_background->GetHeight()/4))
				&& (icon_positions[dragging_icon_index].y + delta_y >= inven_back_pos.y - (shop_background->GetWidth()/3)&& icon_positions[dragging_icon_index].y + delta_y <= inven_back_pos.y + (shop_background->GetWidth()/3)))
			{
				is_on_inven = true;
			}
		}
	}

	// 아이콘 드로잉
	for (int i = 0; i < icon_positions.size(); i++) {
		if (i != dragging_icon_index || !is_dragging) {
			// 드래그 중인 아이콘은 실시간으로 업데이트되므로 제외하고 그릴 준비
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

	Engine::GetRender().AddDrawCall(icon_draw_calls[0], DrawLayer::DrawUI);
	Engine::GetRender().AddDrawCall(icon_draw_calls[1], DrawLayer::DrawUI);
	Engine::GetRender().AddDrawCall(icon_draw_calls[2], DrawLayer::DrawUI);
}





void Shop::Shop_button_draw()
{
	DrawSettings settings;
	settings.is_UI = true;
	settings.do_blending = true;

	botton_matrix = mat3::build_translation({ 300 , (Engine::window_height) / 2.2f - (float)(pick * 140) }) * mat3::build_scale(1.0f);

	if (Engine::Instance().GetInput().KeyJustPressed(CS230::Input::Keys::Up))
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

	if (Engine::Instance().GetInput().KeyJustPressed(CS230::Input::Keys::Down))
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

	draw_call =
	{
		shop_button,                       // Texture to draw
		&botton_matrix,                          // Transformation matrix
		Engine::GetShaderManager().GetDefaultShader(), // Shader to use
		nullptr,
		settings
	};

	Engine::GetRender().AddDrawCall(draw_call, DrawLayer::DrawUI);
}

void Shop::Inventory_Back_draw()
{
	DrawSettings settings;
	settings.is_UI = true;
	settings.do_blending = true;

	inv_back_matrix = mat3::build_translation({ inven_back_pos }) * mat3::build_scale(0.7f) * mat3::build_rotation(3.141592f / 2.0f);

	draw_call =
	{
		shop_background,                       // Texture to draw
		&inv_back_matrix,                          // Transformation matrix
		Engine::GetShaderManager().GetDefaultShader(), // Shader to use
		nullptr,
		settings
	};

	Engine::GetRender().AddDrawCall(draw_call, DrawLayer::DrawUI);
}

void Shop::Inventory_Icon_draw()
{
	DrawSettings settings;
	settings.is_UI = true;
	settings.do_blending = true;

	if (skill_ptr->GetInventory().size() >= 1)
	{
		inv_icon_matrix.clear();
		inv_icon_draw_calls.clear();

		for (int i = 0; i < skill_ptr->GetInventory().size(); i++)
		{
			// 아이콘 위치 및 크기 설정
			inv_icon_matrix.emplace_back(mat3::build_translation({
				defualt_botton_direction.x + inven_back_pos.x,
				defualt_botton_direction.y - ((i + 1) * 140)
				}) * mat3::build_scale(0.4f));

			// DrawCall 생성
			inv_icon_draw_calls.emplace_back(
				draw_call=
				{
				shop_icon,
				&inv_icon_matrix.back(),
				Engine::GetShaderManager().GetDefaultShader(),
				nullptr,
				settings
				});
		}

		// DrawCall 추가
		for (int i = 0; i < skill_ptr->GetInventory().size(); i++) {
			Engine::GetRender().AddDrawCall(inv_icon_draw_calls[i], DrawLayer::DrawUI);
		}
	}
}

