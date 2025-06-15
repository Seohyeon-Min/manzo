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
#include "../Engine/Timer.h"
#include "FishEcosystem.h"
#include "../Engine/Camera.h"
#include "Background.h"
#include "Fish.h"
#include "Shop.h"
#include "Module.h"

class Reef;
class Ship;
class Boss;

class Mode1 : public GameState {
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
    Boss* boss_ptr_e;   
    Boss* boss_ptr_y;
    Cam* camera;
    Beat* beat_system;
    Module* module;

    bool Isboss = false;
    bool soundPlaying = false;
    bool replay = false;
    bool playing = false;
    const float maxDistance = 4000.0f;
    std::vector<std::pair<int, int>> BossFirstPos_e;
    std::vector<std::pair<int, int>> BossFirstPos_y;
    vec3 bossPosition_e;
    vec3 bossPosition_y;
    vec3 previousPosition = { 0,0,0 };
    vec3 smoothShipPosition = { 0,0,0 };

    bool firstBuy = false;
    bool secondBuy = false;
    bool mapChanged = false;

    //Map boundary
    Math::rect level1_boundary = { {700.f, 0},{4500.f, -5500.f} };
    Math::rect level1_camera_boundary = { {700.f, 0},{4400.f, -6800.f} };
    Math::rect level4_boundary = { {-2000.f, -6200.f},{3500.f, -11000.f} };

};
