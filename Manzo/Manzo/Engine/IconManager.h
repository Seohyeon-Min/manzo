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
	void Unload();

	void LoadIconList();
	void AddIcon(std::string alias, vec2 position, float scale, bool drag = true, bool change_pos = false, bool interaction = false, bool draw = false, bool moving = false);
	void RemoveAllIcon();
	bool IsCollidingWith(std::string obj1, std::string obj2);

	void HideIcon(std::string alias);
	void ShowIcon(std::string alias);

	void SetIconPosition(std::string alias, vec2 newPosition);
	vec2 GetIconPosition(std::string obj1, std::string obj2);

	Icon* GetCollidingIcon(Icon& icon);
	Icon* GetCollidingIconWithMouse(vec2 mousePosition);

	std::list<Icon*> GetIconList() { return icons;}

private:
	std::list<Icon*> icons;
	std::unordered_map<std::string, std::filesystem::path> icon_list;
};