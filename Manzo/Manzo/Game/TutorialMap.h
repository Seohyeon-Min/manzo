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
#include "..\Engine\GameObjectManager.h"
#include "GameOption.h"

class Beat;
class Background;
class Ship;

class Tutorial : public GameState {
public:
    Tutorial();
    void Load() override;
    void Update(double dt) override;
    void FixedUpdate(double dt) override;
    void Unload() override;
    void Draw() override;

    std::string GetName() override {
        return "Tutorial";
    }

private:
    Beat* beat_system;
    Background* background;
    Ship* ship_ptr;
    GameOption* option;

    float default_min_radius = 0.0f;
    float default_max_radius = 0.0f;
    float variable_min_radius = 0.0f;
    float variable_max_radius = 0.0f;
    bool playing = false;
};