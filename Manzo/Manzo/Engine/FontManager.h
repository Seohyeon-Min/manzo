#pragma once
#include "Font.h"

class FontManager
{
public:
	FontManager();
	~FontManager();
	void AddFontType(const char* file_path);
	void PrintText(const char* text, vec2 location, float angle, float size, vec3 color);

private:
	label_text_store all_labels;
	const GLShader* shader;
	std::vector<const char*> text_list;
	bool added = false;
};