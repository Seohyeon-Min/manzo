#include "IconManager.h"
#include "Icon.h"

#include <iostream>
#include <fstream>
#include <string>

std::vector<Icon*> icons;

IconManager::IconManager()
{
}

IconManager::~IconManager()
{
	RemoveAllIcon();
	icon_list.clear();
}

void IconManager::LoadIconList()
{
	std::ifstream file("assets/scenes/Icon.csv");
	if (!file.is_open()) {
		std::cerr << "Error: Cannot open icon list file" << std::endl;
		return;
	}

	std::string line, alias, path;
	std::getline(file, line);

	while (std::getline(file, line)) {
		std::stringstream linestream(line);

		if (std::getline(linestream, alias, ',') && std::getline(linestream, path, ',')) {
			icon_list[alias] = path;
		}
	}

	file.close();
	std::cout << "Icon list loaded successfully!" << std::endl;
}


void IconManager::AddIcon(std::string alias, vec2 position, float scale, bool drag, bool change_pos, bool interaction)
{
	auto it = icon_list.find(alias);
	if (it != icon_list.end()) {
		for (const auto& icon : icons)
		{
			if (icon->GetAlias() == alias && icon->GetPosition() == position && icon->GetScale() == scale)
			{
				return;
			}
		}
		Icon* newIcon = new Icon(it->first, it->second, position, scale, drag, change_pos, interaction);
		icons.push_back(newIcon);
	}
	else
	{
		std::cerr << "Error: Icon alias '" << alias << "' not found!" << std::endl;
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

bool IconManager::IsCollidingWith(std::string obj1, std::string obj2)
{
	std::vector<Icon*> icons1;
	std::vector<Icon*> icons2;

	for (auto icon : icons)
	{
		if (icon->GetAlias() == obj1)
		{
			icons1.push_back(icon);
		}
		else if (icon->GetAlias() == obj2)
		{
			icons2.push_back(icon);
		}
	}

	for (auto icon1 : icons1)
	{
		for (auto icon2 : icons2)
		{
			if (icon1->IsCollidingWith(icon2))
			{
				return true;
			}
		}
	}

	if (icons1.empty() || icons2.empty()) {
		return false;
	}

	return false;
}

void IconManager::RemoveIcon(std::string alias)
{
	for (auto icon : icons)
	{
		if (icon->GetAlias() == alias)
		{
			icon->SetDraw(false);
		}
	}
}

void IconManager::SetIconPosition(std::string alias, vec2 newPosition)
{
	Icon* targetIcon = nullptr;
	for (Icon* icon : icons)
	{
		if (icon->GetAlias() == alias)
		{
			targetIcon = icon;
		}
	}

	if (targetIcon)
	{
		targetIcon->SetPosition(newPosition);
		targetIcon->position = newPosition;
	}
}

vec2 IconManager::GetIconPosition(std::string obj1, std::string obj2)
{
	std::vector<Icon*> icons1;
	std::vector<Icon*> icons2;

	for (auto icon : icons)
	{
		if (icon->GetAlias() == obj1)
		{
			icons1.push_back(icon);
		}
		else if (icon->GetAlias() == obj2)
		{
			icons2.push_back(icon);
		}
	}

	for (auto icon1 : icons1)
	{
		for (auto icon2 : icons2)
		{
			if (icon1->IsCollidingWith(icon2))
			{
				return icon1->GetPosition();
			}
		}
	}

	return { 0,0 };
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
	for (Icon* icon : icons)
	{
		if (icon->IsCollidingWith(mousePosition))
		{
			return icon;
		}
	}
	return nullptr;
}