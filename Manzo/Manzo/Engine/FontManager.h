#pragma once
#include "Font.h"

class FontManager
{
public:
	FontManager();
	void AddFontType(const char* file_path);
	void PrintText(const char* text, vec2 location, float angle, float size, vec3 color);
	void SetBuffer();

private:
	label_text_store all_labels;
	const GLShader* shader;
	bool added = false;
};