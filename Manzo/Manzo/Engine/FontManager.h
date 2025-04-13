#pragma once

#include "ShaderManager.h"
#include "Font.cpp"
#include "vec3.h"

enum FontType
{
	Bold,
	Thin,
	AlumniSans_Medium,
	AlumniSans_Light,
	VeryThin,
	Count
};

enum FontAlignment
{
	LEFT,
	CENTER,
	RIGHT
};

static int num_font = 0;

class FontManager
{
public:
	FontManager();
	~FontManager();

	void AddFontType(const char* file_path);
	void PrintText(FontType font, FontAlignment align, std::string txt, vec2 position, float scale, vec3 color, float alpha, bool in_world = true);
	Font* loadFont(const std::string& filename, float worldSize = 1.0f, bool hinting = false);

private:
	FT_Library library;
	GLShader* shader;
	std::unordered_map<FontType, Font*> font_list;
};