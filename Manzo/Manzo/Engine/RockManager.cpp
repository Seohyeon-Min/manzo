/*
Copyright (C) 2023 DigiPen Institute of Technology
Reproduction or distribution of this file or its contents without
prior written consent is prohibited
File Name:  Rock.cpp
Project:    Manzo
Author:     Won Kim
Created:    Sept 13, 2024
*/
#include "RockManager.h"


CS230::RockManager::RockManager(const std::filesystem::path& sprite_file) {
    Load(sprite_file);
}

void CS230::RockManager::Load(const std::filesystem::path& sprite_file) {


    if (sprite_file.extension() != ".rck") {
        throw std::runtime_error(sprite_file.generic_string() + " is not a .rck file");
    }
    std::ifstream in_file(sprite_file);

    if (in_file.is_open() == false) {
        throw std::runtime_error("Failed to load " + sprite_file.generic_string());
    }

    std::string text;
    in_file >> text;

    while (in_file.eof() == false) {
        if (text == "rock") {
            in_file >> point.x;
            in_file >> point.y;
            rect.point_1 = point;

            in_file >> point.x;
            in_file >> point.y;
            rect.point_2 = point;

            rocks.push_back(rect);
            Engine::GetGameStateManager().GetGSComponent<CS230::GameObjectManager>()->Add(new Rock(rect));
        }
        // later... random rock appears
        else if (text == "random") {

        }
        else {
            Engine::GetLogger().LogError("Unknown command: " + text);
        }
        in_file >> text;
    }
}
