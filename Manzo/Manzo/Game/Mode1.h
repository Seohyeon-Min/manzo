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
#include "FishEcosystem.h"
#include "../Engine/Camera.h"
#include "Background.h"
#include "Skill.h"


class Reef;
class Ship;
class Skillsys;
class Skillsys::Skill_Net;
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
    Reef* reef;
    Ship* ship_ptr;
    FishGenerator* fishGenerator;
    Fish* fish;
    Skillsys* skill_ptr;
    Skillsys::Skill_Net* Net_ptr;
    Background* background;
    CS230::Cam* camera;

    int money = 0;
};
