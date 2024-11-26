#include "Shop.h"


Shop::Shop()
{
	skill_ptr = static_cast<Skillsys*>(Engine::Instance().GetTmpPtr());
	shop_background = Engine::GetTextureManager().Load("assets/images/temp_box1.png");
	shop_button = Engine::GetTextureManager().Load("assets/images/temp_box3.png");
	shop_icon = Engine::GetTextureManager().Load("assets/images/temp_box2.png");
	for (int i = 1; i < 4; i++)
	{
		icon_matrix.push_back(mat3::build_translation({ -300 , (Engine::window_height) / 1.6f - (float) (i*200) }) * mat3::build_scale(0.5f));
	}
	icon_draw_calls.resize(3);
	//Read save file?
}


void Shop::Update(double dt)
{
	if (Engine::GetInput().KeyJustPressed(CS230::Input::Keys::Y))
	{
		shop_available = true;
		std::cout << "Shop active" << std::endl;
	}

	if (shop_available) // Can buy something
	{
		Shop_Back_draw(); // draw shop background
		Shop_icon_draw();
		Shop_button_draw();

		if (pick == First)
		{
			if (Engine::Instance().GetInput().KeyJustPressed(CS230::Input::Keys::Enter))
			{
				Buy(skill_ptr->Net, Net_Money);
			}
		}

		if (pick == Second)
		{
			if (Engine::Instance().GetInput().KeyJustPressed(CS230::Input::Keys::Enter))
			{
				Buy(skill_ptr->Light, Net_Money);
			}
		}
	}
}

void Shop::Buy(Skillsys::Skill_list skill,int input)
{
	skill_ptr->AddSkill(skill, input);
	shop_available = false;
	std::cout << "Shop inactive" << std::endl;
}

void Shop::Shop_Back_draw()
{
	DrawSettings settings;
	settings.is_UI = true;
	settings.do_blending = true;

	back_matrix = mat3::build_translation({ 0 , Engine::window_height / 12 }) * mat3::build_scale(1.0f);

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

	for (int i = 0; i < 3; i++)
	{
		icon_draw_calls[i] =
		{
			shop_icon,                       // Texture to draw
			&icon_matrix[i],                          // Transformation matrix
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

	botton_matrix = mat3::build_translation({ 0 , (Engine::window_height) / 1.6f - (float)(pick * 200) }) * mat3::build_scale(1.0f);

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