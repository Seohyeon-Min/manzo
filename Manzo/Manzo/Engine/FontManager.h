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
	Count
};

static int num_font = 0;

class FontManager
{
public:
	FontManager();
	~FontManager();

	void AddFontType(const char* file_path);
	void PrintText(FontType font, std::string txt, vec2 position, float scale, vec3 color, float alpha, bool in_world = true);
	std::unique_ptr<Font> loadFont(const std::string& filename, float worldSize = 1.0f, bool hinting = false);

private:
	FT_Library library;
	GLShader* shader;
	std::unique_ptr<Font> mainFont;
	std::unordered_map<FontType, std::unique_ptr<Font>> font_list;
};