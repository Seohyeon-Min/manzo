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
	mainFont = std::move(font);
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

void FontManager::PrintText(std::string txt, vec2 position, vec3 color)
{
	FT_Error error = FT_Init_FreeType(&library);
	if (error) {
		std::cerr << "ERROR: failed to initialize FreeType" << std::endl;
		return;
	}

	shader = Engine::GetShaderManager().GetShader("font_shader");

	mainFont->drawSetup();

	GLuint location = glGetUniformLocation(0, "color");
	glUniform4f(location, 1.0f, 1.0f, 1.0f, 1.0f);  //Font Color

	mainFont->draw(position.x, position.y, txt);

	shader->Use(false);
}
