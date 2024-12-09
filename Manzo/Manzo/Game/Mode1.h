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
#include "../Engine/UIManager.h"
#include "../Engine/Timer.h"
#include "FishEcosystem.h"
#include "../Engine/Camera.h"
#include "Background.h"
#include "Skill.h"
#include "Fish.h"
#include "GodRay.h"

class Reef;
class Ship;
class Boss;

class Skillsys;

class Mode1 : public CS230::GameState {
public:
    Mode1();
    void Load() override;
    void Update(double dt) override;
    void FixedUpdate(double dt) override;
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
    Background* background;
    Boss* boss_ptr;
    UIManager* ui_manager;
    CS230::Cam* camera;
    Beat* beat_system;
    GodRay* god_ray;
    Skillsys* skill_ptr;
    Skillsys::Skill_Net* Net_ptr;

    bool Isboss = false;
    bool soundPlaying = false;
    bool replay = false;
    bool playing = false;
    const float maxDistance = 350.0f;
    std::vector<std::pair<int, int>> BossFirstPos;
    vec3 bossPosition;
    vec3 previousPosition = { 0,0,0 };
    vec3 smoothShipPosition = { 0,0,0 };
};
