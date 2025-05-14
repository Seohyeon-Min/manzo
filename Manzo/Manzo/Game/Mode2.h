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
#include "..\Engine\IconManager.h"
#include "../Engine/Timer.h"
#include "Background.h"
#include "Fish.h"
#include "Shop.h"
#include "DialogBox.h"
#include "Player.h"
#include "Inventory.h"
#include "Module.h"
#include "ScenarioComponent.h"
#include "PopUp.h"
#include "FishEcosystem.h"

class Fish;

class Ship;

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
    Shop* shop_ptr;
    Dialog* dialog_ptr;
    Player* player_ptr;
    Inven* inven_ptr;
    Module* module_ptr;
    ScenarioComponent* scenario;
    PopUp* sell_popup;
    PopUp* today_fish_popup;
    FishGenerator* fishGenerator;

    float y_limit = 15.0f;
    float frequency = 0.9f;
    float time = 0.0f;
    bool playing = false;
    bool playing_replay = false;
    bool flag = false;
    bool isLoaded = false;
    char n;
};