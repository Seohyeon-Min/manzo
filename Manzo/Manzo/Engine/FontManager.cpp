#include "FontManager.h"
#include "ShaderManager.h"
#include "Engine.h"
#include "Font.h"

FontManager::FontManager()
{
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

	std::array<float, 3> color_data = { color.x, color.y, color.z };
	std::span<float, 3> color_span(color_data);
	shader->SendUniform("text_color", color_span);

	all_labels.add_text(text, location, angle, size);
	all_labels.set_buffers();

	all_labels.paint_text();
}
