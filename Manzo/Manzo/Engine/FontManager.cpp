#include "FontManager.h"
#include "ShaderManager.h"
#include "Engine.h"
#include "Font.h"

FontManager::FontManager()
{
}

FontManager::~FontManager()
{
	text_list.clear();
}

void FontManager::AddFontType(const char* file_path)
{
	if (!file_path)
	{
		std::cerr << "No file exists in that path" << std::endl;
		return;
	}

	shader = Engine::GetShaderManager().LoadShader("font_shader", "assets/shaders/font_shader.vert", "assets/shaders/font_shader.frag");
	all_labels.init(file_path);
}

void FontManager::PrintText(const char* text, vec2 location, float angle, float size, vec3 color)
{
	shader = Engine::GetShaderManager().GetShader("font_shader");
	
	shader->Use(true);

	shader->SendUniform("zoomscale", 1.0f);
	shader->SendUniform("u_Texture", 0);

	/*std::array<float, 3> color_data = { color.x, color.y, color.z };
	shader->SendUniform("text_color", std::span(color_data));*/

	auto it = std::find(text_list.begin(), text_list.end(), text);
	if (it == text_list.end()) // Text not found
	{
		all_labels.add_text(text, location, angle, size, color);
		all_labels.set_buffers();
		text_list.push_back(text);
	}

	all_labels.paint_text();
}
