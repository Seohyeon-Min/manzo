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


void IconManager::AddIcon(std::string alias, vec2 position, float scale, bool drag, bool change_pos)
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
        Icon* newIcon = new Icon(it->first, it->second, position, scale, drag, change_pos);
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
