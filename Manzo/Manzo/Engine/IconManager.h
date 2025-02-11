#pragma once

#include"vec2.h"

#include <unordered_map>
#include <filesystem>

class IconManager
{
public:
	IconManager();
	~IconManager();

	void LoadIconList();
	void AddIcon(const char* alias, vec2 position, float scale);

private: 
	std::unordered_map<std::string, std::filesystem::path> icon_list;
};