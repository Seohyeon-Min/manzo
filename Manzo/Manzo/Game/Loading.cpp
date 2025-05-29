/*
Copyright (C) 2023 DigiPen Institute of Technology
Reproduction or distribution of this file or its contents without
prior written consent is prohibited
File Name:  Loading.cpp
Project:    Manzo
Author:     Won Kim
Created:    May 7, 2025
*/

#include "../Engine/Engine.h"
#include "../Engine/MapManager.h"

#include "Loading.h"
#include "States.h"


Loading::Loading() {}

void Loading::Load()
{
	//Load First Map
	//AddGSComponent(new MapManager());
	//GetGSComponent<MapManager>()->AddMapFile("assets/maps/fixing2.svg");
	//GetGSComponent<MapManager>()->LoadFirstMap();
}

void Loading::Update(double dt) {
    if (!GetGSComponent<MapManager>()->GetCurrentMap()->IsLevelLoaded()) {
        GetGSComponent<MapManager>()->GetCurrentMap()->ParseSVG();
    }
    else {
        Engine::GetGameStateManager().SetNextGameState(static_cast<int>(States::Mode1));
    }

    // Draw Loading Screen
    // Loading bar would be great
}


void Loading::FixedUpdate(double dt) {
}

void Loading::Unload() {
}

void Loading::Draw() {
}