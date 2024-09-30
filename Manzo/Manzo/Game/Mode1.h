/*
Copyright (C) 2023 DigiPen Institute of Technology
Reproduction or distribution of this file or its contents without
prior written consent is prohibited
File Name:  Mode1.h
Project:    CS230 Engine
Author:     Jonathan Holmes
Created:    March 8, 2023
*/

#pragma once
#include "../Engine/GameState.h"
#include "../Engine/Sprite.h"
#include "..\Engine\GameObjectManager.h"
//#include "../Engine/Fonts.h"
#include "../Engine/Timer.h"

class Fish;
class Reef;
class Ship;

constexpr double fish_timer = 2.0;

class Mode1 : public CS230::GameState {
public:
    Mode1();
    void Load() override;
    void Update(double dt) override;
    void Unload() override;
    void Draw() override;

    std::string GetName() override {
        return "Mode1";
    }
private:
    double counter = 0.0;
    double swimming_range = 15.0;
    CS230::Timer* timer;
    std::vector<Fish*> fishList;
    Fish* fish;
    Reef* reef;
    Ship* ship_ptr;
    int money = 0;
};
