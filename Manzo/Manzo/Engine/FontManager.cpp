#include "FontManager.h"
#include "ShaderManager.h"
#include "Engine.h"
#include "Font.h"

int FontManager::fontNum = 0;

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
	all_labels[fontNum++].init(file_path);
}

void FontManager::PrintText(int ft, const char* text, vec2 location, float angle, float size, vec3 color)
{
    shader = Engine::GetShaderManager().GetShader("font_shader");

    shader->Use(true);
    shader->SendUniform("u_Texture", 0);

    std::string textStr(text);

    auto it = std::find(text_list.begin(), text_list.end(), textStr);
    if (it == text_list.end()) 
    {
        all_labels[ft].add_text(textStr, location, angle, size, color);
        all_labels[ft].set_buffers();
        text_list.push_back(textStr);
    }

    all_labels[ft].paint_text();
}