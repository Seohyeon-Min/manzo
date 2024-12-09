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

//struct�� ��ü ������ �����س��°� ����?
//�����ڿ��� ���� �� ��Ʈ�� �� �ε��س��� draw���� �׿� �´� ������ �ش��ϴ� �� ã�Ƽ� �׸���?