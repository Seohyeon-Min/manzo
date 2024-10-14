/*
Copyright (C) 2023 DigiPen Institute of Technology
Reproduction or distribution of this file or its contents without
prior written consent is prohibited
File Name:  Mode1.cpp
Project:    CS230 Engine
Author:     Jonathan Holmes
Created:    March 8, 2023
*/

#include "../Engine/Engine.h"
#include "../Engine/Timer.h"
#include "../Engine/ShowCollision.h"
#include "../Engine/AudioManager.h"

#include "States.h"
#include "Mode1.h"
#include "BeatSystem.h"
#include "Background.h"
#include "Ship.h"
#include "Fish.h"
#include "Reef.h"

#include <iostream>

Mode1::Mode1()
{
    timer = new CS230::Timer(fish_timer);
}


void Mode1::Load() {

#ifdef _DEBUG
    AddGSComponent(new CS230::ShowCollision());
#else
#endif
    // compenent
    AddGSComponent(new CS230::GameObjectManager());
    //AddGSComponent(new Background());
    AddGSComponent(new Beat());
    AddGSComponent(new AudioManager());
    //GetGSComponent<Background>()->Add("assets/images/temp_back.png", 0.25);

    //// ship
    ship_ptr = new Ship({ 0, 0 });
    GetGSComponent<CS230::GameObjectManager>()->Add(ship_ptr);

    //// camera
    AddGSComponent(new CS230::Camera({ {1280 / 2 , 720 / 2 }, {1280 / 2, 720 / 2 } }));
    vec2 playerPosition = ship_ptr->GetPosition();
    GetGSComponent<CS230::Camera>()->SetPosition({ playerPosition.x - 1280 / 2, playerPosition.y - 720 / 2 });
    //GetGSComponent<CS230::Camera>()->SetLimit({ { 0, 0}, {  1680 , 5000} });

    //// audio
    Mix_Music* sample = GetGSComponent<AudioManager>()->LoadMusic("assets/audios/basic_beat_100_4.wav", "sample");
    if (sample) {
        GetGSComponent<AudioManager>()->PlayMusic(sample, -1);
    }
}

void Mode1::Update(double dt) {
    UpdateGSComponents(dt);
    GetGSComponent<CS230::GameObjectManager>()->UpdateAll(dt);
    GetGSComponent<CS230::Camera>()->Update(ship_ptr->GetPosition());

    if (Engine::GetInput().KeyJustPressed(CS230::Input::Keys::Q)) {
        Engine::GetGameStateManager().SetNextGameState(static_cast<int>(States::Mode2));
        
    }

}

void Mode1::Draw() {
    GetGSComponent<CS230::GameObjectManager>()->DrawAll();
}

void Mode1::Unload() {
    ship_ptr = nullptr;
	GetGSComponent<CS230::GameObjectManager>()->Unload();
	ClearGSComponents();
}
