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
#include <to_span.h>
#include "Camera.h"

FontManager::FontManager() {
	if (FT_Init_FreeType(&library)) {
		std::cerr << "ERROR: failed to initialize FreeType" << std::endl;
	}
}

FontManager::~FontManager() {
	for (auto& pair : font_list) {
		delete pair.second;  // Font* 포인터 메모리 해제
	}

	FT_Done_FreeType(library);  // FreeType 자원 해제
}

void FontManager::AddFontType(const char* file_path)
{
	shader = Engine::GetShaderManager().LoadShader("font_shader", "assets/shaders/font_shader.vert", "assets/shaders/font_shader.frag");
	auto font = loadFont(file_path, 1.0f);
	if (!font) return;
	font_list[(FontType)num_font++] = std::move(font);
}

Font* FontManager::loadFont(const std::string& filename, float worldSize, bool hinting)
{
	std::string error;
	FT_Face face = Font::loadFace(library, filename, error);
	if (error != "") {
		std::cerr << "[font] failed to load " << filename << ": " << error << std::endl;
		return nullptr;
	}

	return new Font(face, worldSize, hinting);
}

void FontManager::PrintText(FontType font, std::string txt, vec2 position, float scale, vec3 color, float alpha, bool in_world)
{
	shader = Engine::GetShaderManager().GetShader("font_shader");

	shader->Use(true);

	shader->SendUniform("color", color.x, color.y, color.z);
	shader->SendUniform("alphaV", alpha);

	font_list[font]->setWorldSize(scale);
	font_list[font]->drawSetup(shader);

	if (in_world)
	{
		mat3 model_to_world = mat3::build_translation(position);
		mat3 WORLD_TO_NDC = Engine::GetGameStateManager().GetGSComponent<Cam>()->world_to_ndc;
		const mat3 model_to_ndc = WORLD_TO_NDC * model_to_world;
		vec3 ndcPos = model_to_ndc * vec3(position.x, position.y, 1.0);

		font_list[font]->draw(ndcPos, txt);
	}
	else
	{
		font_list[font]->draw(position.x, position.y, txt);
	}

}
