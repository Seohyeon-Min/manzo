#include "IconManager.h"
#include "Icon.h"

#include <iostream>
#include <fstream>
#include <memory>
#include <string>

std::vector<std::unique_ptr<Icon>> icons;

IconManager::IconManager()
{
}

IconManager::~IconManager()
{
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


void IconManager::AddIcon(const char* alias, vec2 position, float scale)
{
    auto it = icon_list.find(alias);
    if (it != icon_list.end()) {
        icons.push_back(std::make_unique<Icon>(it->second, position, scale));
    }
    else {
        std::cerr << "Error: Icon alias '" << alias << "' not found!" << std::endl;
    }
}
