#include "Shop.h"
#include "Dragging.h"
#include "..\Engine\GameObject.h"

static bool is_on_inven = false;
static bool is_on_shop = false;
static bool Ready_to_buy = false;
static bool Ready_to_sell = false;

Shop::Shop(vec2 postion) : GameObject(postion)
{
	Read_Shop_Csv("assets/scenes/shop.csv");
	AddGOComponent(new Sprite("assets/images/window.spt", this));

	int count = 1;
	for (auto& info : shop_infos)
	{
		std::cout << count << "th: " << std::endl;
		std::cout << "name: " << info.name << std::endl;
		std::cout << "icon name: " << info.icon << std::endl;
		std::cout << "price : " << info.price << std::endl;
		std::cout << "script : " << info.script << std::endl;
		++count;
		is_on_shop = true;
	}
	std::cout << this->GetAABB().Size().height << " , " << this->GetAABB().Size().width << std::endl;
}

Shop::~Shop()
{
}


void Shop::Update(double dt)
{
	GameObject::Update(dt);

	if (Engine::GetInput().KeyJustPressed(Input::Keys::Y) && !shop_available)
	{
		shop_available = true;
		std::cout << "Shop active" << std::endl;
		is_on_shop = true;
	}

	if (shop_available) // Is shop available?
	{
		/*
		* 1. 일단 파일 내에 있는걸 읽어오는지 확인하기 // 되네
		* 2. 아이콘 먼저 띄워보기 // 이거 해야함
		* 3. 아이콘 갖고오면 일단 그 문자열을 가져오게 // 그냥 다 되는거같은데?
		* 4. 드래그 해야하는데 드래그 계속 안됨(이것만 되면 될거같은데) -> 드래그 시도는 하는데 아이콘이 움직이질 않음 왜 와이?
		*/
		//Shop_Back_draw();

		Engine::GetIconManager().AddIcon("fish3", { 0, 0 }, 2.0f);

		if (Engine::GetInput().KeyJustPressed(Input::Keys::E) && shop_available)
		{
			shop_available = false;
			std::cout << "Shop inactive" << std::endl;
			Engine::GetIconManager().RemoveAllIcon();
			is_on_shop = false;
		}

		int count = 1;
		for (auto& info : shop_infos)
		{
			Engine::GetIconManager().AddIcon(info.icon, { 500 , 300.0f - (120.0f * count) }, 1.0f);
			count++;
		}
		count = 1;

		Ready_to_sell = true;
		if (Engine::GetGameStateManager().GetGSComponent<Dragging>()->GetCurrentDraggingIcon() != nullptr && Ready_to_sell)
		{
			Icon* draggedIcon = Engine::GetGameStateManager().GetGSComponent<Dragging>()->GetCurrentDraggingIcon();
			std::cout << "you dragged... : " << draggedIcon->GetAlias() << std::endl;
			std::string icon_name = draggedIcon->GetAlias();

			if (Engine::GetIconManager().IsCollidingWith("fish3", icon_name))
			{
				std::cout << "For check(collided)" << std::endl;
				for (auto it = shop_infos.begin(); it != shop_infos.end(); ++it)
				{
					if (it->icon == icon_name)
					{
						int money = Engine::GetGameStateManager().GetGSComponent<Fish>()->GetMoney();
						Engine::GetGameStateManager().GetGSComponent<Fish>()->SetMoney(money + it->price);
						std::cout << "Sell Complete, left money is : "
							<< Engine::GetGameStateManager().GetGSComponent<Fish>()->GetMoney() << std::endl;
							Engine::GetIconManager().RemoveAllIcon();

						shop_infos.erase(it);  // 요소 삭제
						std::cout << "Item erased, exiting loop." << std::endl;
						Ready_to_sell = false;
						break;  // 삭제 후 반복 종료
					}
				}

				int count = 1;
				for (auto& info : shop_infos)
				{
					std::cout << count << "th: " << std::endl;
					std::cout << "name: " << info.name << std::endl;
					std::cout << "icon name: " << info.icon << std::endl;
					std::cout << "price : " << info.price << std::endl;
					std::cout << "script : " << info.script << std::endl;
					++count;
					is_on_shop = true;
				}
			}
		}
	}
}

void Shop::Draw(DrawLayer drawlayer)
{
	GameObject::Draw();
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

	DrawCall draw_call = 
	{
		shop_background,                       // Texture to draw
		&back_matrix,                          // Transformation matrix
		Engine::GetShaderManager().GetDefaultShader(), // Shader to use
		//nullptr,
		//settings
	};

	draw_call.settings.is_camera_fixed = true;
	draw_call.settings.do_blending = true;
	draw_call.sorting_layer = DrawLayer::DrawFirst;

	Engine::GetRender().AddDrawCall(std::make_unique<DrawCall>(draw_call));
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
void Shop::Read_Shop_Csv(const std::string& filename)
{
	std::ifstream file(filename);
	if (!file.is_open()) 
	{
		std::cerr << "Failed to open file: " << filename << std::endl;
		return;
	}

	std::string line, cell;
	std::getline(file, line);

	while (std::getline(file, line)) 
	{
		std::stringstream linestream(line);
		shop_info shop_i;

		std::getline(linestream, cell, ',');
		shop_i.name = cell;

		std::getline(linestream, cell, ',');
		shop_i.icon = cell;

		std::getline(linestream, cell, ',');
		shop_i.price = static_cast<int>(std::stol(cell));

		std::getline(linestream, cell, ',');
		shop_i.script = cell;
		
		shop_infos.push_back(shop_i);
	}
	file.close();
	std::cout << "Shop loaded successfully." << std::endl;
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

