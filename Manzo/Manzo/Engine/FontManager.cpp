#include <cmath>
#include <cstdint>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <ft2build.h>
#include "freetype.h"
#include "FontManager.h"
#include "Engine.h"

FontManager::FontManager() {
	if (FT_Init_FreeType(&library)) {
		std::cerr << "ERROR: failed to initialize FreeType" << std::endl;
	}
}

FontManager::~FontManager() {
	FT_Done_FreeType(library); // Free FreeType resources
}

void FontManager::AddFontType(const char* file_path)
{
	shader = Engine::GetShaderManager().LoadShader("font_shader", "assets/shaders/font_shader.vert", "assets/shaders/font_shader.frag");
	auto font = loadFont(file_path, 1.0f);
	if (!font) return;
	font_list[(FontType)num_font++] = std::move(font);
}

std::unique_ptr<Font> FontManager::loadFont(const std::string& filename, float worldSize, bool hinting)
{
	std::string error;
	FT_Face face = Font::loadFace(library, filename, error);
	if (error != "") {
		std::cerr << "[font] failed to load " << filename << ": " << error << std::endl;
		return std::unique_ptr<Font>{};
	}

	return std::make_unique<Font>(face, worldSize, hinting);
}

void FontManager::PrintText(FontType font, std::string txt, vec2 position, vec3 color, float alpha)
{
	FT_Error error = FT_Init_FreeType(&library);
	if (error) {
		std::cerr << "ERROR: failed to initialize FreeType" << std::endl;
		return;
	}

	shader = Engine::GetShaderManager().GetShader("font_shader");

	shader->Use(true);

	shader->SendUniform("color", color.x, color.y, color.z);
	shader->SendUniform("alphaV", alpha);
	font_list[font]->drawSetup(shader);

	font_list[font]->draw(position.x, position.y, txt);
}
