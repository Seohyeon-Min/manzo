#pragma once

#include"vec2.h"

#include <unordered_map>
#include <filesystem>

class Icon;

class IconManager
{
public:
	IconManager();
	~IconManager();

	void LoadIconList();
	void AddIcon(std::string alias, vec2 position, float scale, bool drag = true, bool change_pos = false, bool interaction = false);
	void RemoveAllIcon();
	bool IsCollidingWith(std::string obj1, std::string obj2);
	//void RemoveIcon(std::string alias);

	Icon* GetCollidingIcon(Icon& icon);
	Icon* GetCollidingIconWithMouse(vec2 mousePosition);

private: 
	std::unordered_map<std::string, std::filesystem::path> icon_list;
};