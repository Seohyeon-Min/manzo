#include "IconManager.h"
#include "Icon.h"

#include <iostream>
#include <fstream>
#include <string>
#include "GameObjectManager.h"

IconManager::IconManager()
{
}

IconManager::~IconManager()
{
	Unload();
}

void IconManager::Unload()
{
	for (Icon* icon : icons)
	{
		icon->Destroy();
		delete icon;   
	}
	icons.clear();
	icon_by_id.clear();
	icon_list.clear();
}

void IconManager::LoadIconList()
{
	std::ifstream file("assets/scenes/Icon.csv");
	if (!file.is_open()) {
		std::cerr << "Error: Cannot open icon list file" << std::endl;
		return;
	}

	std::string line, img_type, path;
	std::getline(file, line);

	while (std::getline(file, line)) {
		std::stringstream linestream(line);

		if (std::getline(linestream, img_type, ',') && std::getline(linestream, path, ',')) {
			icon_list[img_type] = path;
		}
	}

	file.close();
	std::cout << "Icon list loaded successfully!" << std::endl;
}

void IconManager::AddIcon(std::string group, std::string id, std::string img_type, vec2 position, float scale, bool drag, bool change_pos, bool interaction, bool draw, bool moving)
{
	if (icon_by_id.contains(id)) {
		std::cerr << "Warning: Icon ID '" << id << "' already exists." << std::endl;
		return;
	}

	auto it = icon_list.find(img_type);
	if (it != icon_list.end()) {
		Icon* newIcon = new Icon(id, group, it->second, position, scale, drag, change_pos, interaction, draw, moving);
		icons.push_back(newIcon);
		icon_by_id[id] = newIcon;
	}
	else {
		std::cerr << "Error: img_type '" << img_type << "' not found in icon list!" << std::endl;
	}
}


void IconManager::RemoveAllIcon()
{
	for (auto icon : icons)
	{
		icon->Destroy();
	}
	icons.clear();
}

bool IconManager::IsCollidingWith(const std::string& id1, const std::string& id2)
{
	auto it1 = icon_by_id.find(id1);
	auto it2 = icon_by_id.find(id2);

	if (it1 == icon_by_id.end() || it2 == icon_by_id.end()) {
		return false;
	}

	Icon* icon1 = it1->second;
	Icon* icon2 = it2->second;

	return icon1->IsCollidingWith(icon2);
}


void IconManager::HideIconById(const std::string& id)
{
	auto it = icon_by_id.find(id);
	if (it != icon_by_id.end()) {
		it->second->SetHide(true);
	}
}

void IconManager::ShowIconById(const std::string& id)
{
	auto it = icon_by_id.find(id);
	if (it != icon_by_id.end()) {
		it->second->SetHide(false);
	}
}

void IconManager::RemoveIconById(const std::string& id)
{
	auto it = icon_by_id.find(id);
	if (it != icon_by_id.end()) {
		Icon* icon = it->second;
		icon->Destroy();

		icons.erase(std::remove(icons.begin(), icons.end(), icon), icons.end());
		icon_by_id.erase(it);
		delete icon;
	}
}

void IconManager::HideIconByGroup(const std::string& group)
{
	for (Icon* icon : icons)
	{
		if (icon->GetGroup() == group)
		{
			icon->SetHide(true);
		}
	}
}

void IconManager::ShowIconByGroup(const std::string& group)
{
	for (Icon* icon : icons)
	{
		if (icon->GetGroup() == group)
		{
			icon->SetHide(false);
		}
	}
}

void IconManager::SetPositionById(const std::string& id, vec2 newPosition)
{
	auto it = icon_by_id.find(id);
	if (it != icon_by_id.end()) {
		it->second->SetPosition(newPosition);
		it->second->position = newPosition; // if still needed elsewhere
	}
}

void IconManager::SetIconPositionById(const std::string& id, const std::string& target_pos)
{
	auto it1 = icon_by_id.find(id);
	auto it2 = icon_by_id.find(target_pos);

	if (it1 != icon_by_id.end() && it2 != icon_by_id.end()) {
		it1->second->SetPosition(it2->second->GetPosition());
	}
}

vec2 IconManager::GetIconPositionIfColliding(const std::string& id1, const std::string& id2)
{
	auto it1 = icon_by_id.find(id1);
	auto it2 = icon_by_id.find(id2);

	if (it1 != icon_by_id.end() && it2 != icon_by_id.end()) {
		if (it1->second->IsCollidingWith(it2->second)) {
			return it1->second->GetPosition();
		}
	}

	return { 0, 0 };
}

Icon* IconManager::GetCollidingIcon(Icon& icon)
{
	for (Icon* other : icons)
	{
		if (other != &icon && icon.IsCollidingWith(other))
		{
			if ((icon.CanDrag() && !other->CanDrag()) || (!icon.CanDrag() && other->CanDrag()))
			{
				return other;
			}
		}
	}
	return nullptr;
}

Icon* IconManager::GetCollidingIconWithMouse(vec2 mousePosition)
{
	for (auto& [id, icon] : icon_by_id)
	{
		if (icon->IsCollidingWith(mousePosition) && !icon->IsHide())
		{
			return icon;
		}
	}
	return nullptr;
}

vec2 IconManager::GetIconPosition(const std::string& id1)
{
	auto it1 = icon_by_id.find(id1);

	if (it1 != icon_by_id.end()) {
		return it1->second->GetPosition();
	}

	return { 0, 0 };
}

Icon* IconManager::GetIcon(const std::string& id)
{
	auto it = icon_by_id.find(id);

	if (it != icon_by_id.end()) {
		return it->second;
	}

	return nullptr;
}