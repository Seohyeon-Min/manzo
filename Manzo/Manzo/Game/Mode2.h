/*
Copyright (C) 2023 DigiPen Institute of Technology
Reproduction or distribution of this file or its contents without
prior written consent is prohibited
File Name:  Mode2.h
Project:    Manzo
Author:     Won Kim
Created:    Sept 12, 2024
*/

#pragma once
#include "../Engine/GameState.h"
#include "../Engine/Sprite.h"
#include "..\Engine\GameObjectManager.h"
#include "../Engine/Timer.h"
#include "Background.h"
#include "Fish.h"
#include "Shop.h"
#include "DialogBox.h"
#include "Player.h"
#include "Inventory.h"
#include "Module.h"

class Fish;

class Ship;

class Skillsys;

class Mode2 : public GameState {
public:
    Mode2();
    void Load() override;
    void Update(double dt) override;
    void FixedUpdate(double dt) override;
    void Unload() override;
    void Draw() override;

    std::string GetName() override {
        return "Mode2";
    }

private:
    double counter = 0.0;
    double swimming_range = 15.0;
    Timer* timer;
    Ship* ship_ptr;
    Background* background;
    Skillsys* skill_ptr;
    Shop* shop_ptr;
    Dialog* dialog_ptr;
    Player* player_ptr;
    Inven* inven_ptr;
    Module* module_ptr;

    float y_limit = 15.0f;
    float frequency = 0.9f;
    float time = 0.0f;

    bool isLoaded = false;
};