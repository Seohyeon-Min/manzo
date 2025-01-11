#pragma once

#include "ShaderManager.h"
#include "Font.cpp"
#include "vec3.h"

class FontManager
{
public:
	FontManager();
	~FontManager();

	void AddFontType(const char* file_path);
	void PrintText(std::string txt, vec2 position, vec3 color);
	std::unique_ptr<Font> loadFont(const std::string& filename, float worldSize = 1.0f, bool hinting = false);

private:
	FT_Library library;
	GLShader* shader;
	std::unique_ptr<Font> mainFont;
};