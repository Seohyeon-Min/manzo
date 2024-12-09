#pragma once
#include "Font.h"
#include <array>

enum FontType
{
	Bold,
	Thin,
	Count
};

class FontManager
{
public:
	FontManager();
	~FontManager();
	void AddFontType(const char* file_path);
	void PrintText(int ft, const char* text, vec2 location, float angle, float size, vec3 color);

private:
	std::array<label_text_store, FontType::Count> all_labels;
	const GLShader* shader;
	std::vector<std::string> text_list;
	bool added = false;
	static int fontNum;
};

//struct로 전체 정보를 저장해놓는게 좋나?
//생성자에서 내가 쓸 폰트들 다 로드해놓고 draw에서 그에 맞는 정보에 해당하는 걸 찾아서 그리게?