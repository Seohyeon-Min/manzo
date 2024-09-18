/*
Copyright (C) 2023 DigiPen Institute of Technology
Reproduction or distribution of this file or its contents without
prior written consent is prohibited
File Name:  Rock.cpp
Project:    Manzo
Author:     Won Kim
Created:    Sept 13, 2024
*/

#pragma once
#include "GameObject.h"
#include "GameObjectManager.h"
#include "../Game/Rock.h"
#include <string>
#include "Texture.h"
#include "Animation.h"
#include "Component.h"
#include "Collision.h"
#include "engine.h"

namespace CS230 {
    class RockManager :public Component {
    public:
        RockManager(const std::filesystem::path& sprite_file);

        RockManager(const Sprite&) = delete;
        RockManager& operator=(const Sprite&) = delete;

        void Load(const std::filesystem::path& sprite_file);

    private:
        std::vector<Math::irect> rocks;
        Math::irect rect;
        Math::ivec2 point;
    };
}