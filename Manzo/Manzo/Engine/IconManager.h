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

	void AddIcon(std::string group, std::string id, std::string img_type, vec2 position, float scale, bool drag = true, bool change_pos = false, bool interaction = false, bool draw = false, bool moving = false);
	void RemoveAllIcon();

	bool IsCollidingWith(const std::string& id1, const std::string& id2);

	void HideIconById(const std::string& id);

	void ShowIconById(const std::string& id);

	void RemoveIconById(const std::string& id);

	void HideIconByGroup(const std::string& group);
	void ShowIconByGroup(const std::string& group);

	void SetIconPositionById(const std::string& id, vec2 newPosition);
	void SetIconPositionById(const std::string& id, const std::string& target_pos);

	vec2 GetIconPositionIfColliding(const std::string& id1, const std::string& id2);

	Icon* GetCollidingIcon(Icon& icon);

	Icon* GetCollidingIconWithMouse(vec2 mousePosition);
	vec2 GetIconPosition(const std::string& id1);

	std::list<Icon*> GetIconList() { return icons;}

private:
	std::list<Icon*> icons;
	std::unordered_map<std::string, std::filesystem::path> icon_list;
	std::unordered_map<std::string, Icon*> icon_by_id;
};