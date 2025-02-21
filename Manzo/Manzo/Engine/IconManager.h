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
	void AddIcon(std::string alias, vec2 position, float scale, bool drag = true, bool change_pos = false);
	void RemoveAllIcon();

private: 
	std::unordered_map<std::string, std::filesystem::path> icon_list;
};