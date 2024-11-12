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
#include "../Engine/ShowCollision.h"
#include "../Engine/AudioManager.h"
#include <cmath>

#include "States.h"
#include "Background.h"
#include "BeatSystem.h"
#include "Ship.h"
#include "Mode2.h"


#include <iostream>     // for debug

Mode2::Mode2() {}

void Mode2::Load() {

#ifdef _DEBUG
    AddGSComponent(new CS230::ShowCollision());
#else
#endif
    // compenent
    AddGSComponent(new CS230::GameObjectManager());
    AddGSComponent(new Skillsys());
    //AddGSComponent(new Background());
    //GetGSComponent<Background>()->Add("assets/images/temp_back.png", 0.25);

    //// ship
    ship_ptr = new Ship({ 0, -250 });
    GetGSComponent<CS230::GameObjectManager>()->Add(ship_ptr);

    //// camera
    AddGSComponent(new CS230::Cam());
    vec2 playerPosition = ship_ptr->GetPosition();
    GetGSComponent<CS230::Cam>()->SetPosition({ playerPosition.x, 0 });


    //// background
    background = new Background();
    AddGSComponent(background);
    GetGSComponent<Background>()->Add("assets/images/temp_back2.png", 0.25f);


    //// audio
    //Mix_Music* sample = GetGSComponent<AudioManager>()->LoadMusic("assets/audios/basic_beat_100_4.wav", "sample");
    //if (sample) {
    //    GetGSComponent<AudioManager>()->PlayMusic(sample, -1);
    //}

    // skill
    Skill_ptr = GetGSComponent<Skillsys>();
}

void Mode2::Update(double dt) {
    UpdateGSComponents(dt);
    GetGSComponent<CS230::GameObjectManager>()->UpdateAll(dt);
    GetGSComponent<CS230::Cam>()->Update(dt, ship_ptr->GetPosition(), false);
    Skill_ptr->Update(dt);
    GetGSComponent<Skillsys>()->Update(dt);

    //float moving~
    time += float(dt);
    ship_ptr->SetVelocity({ 0, -(y_limit * frequency * std::cos(frequency * float(time))) });

    if (Engine::GetInput().KeyJustPressed(CS230::Input::Keys::Q)) {
        Engine::GetGameStateManager().ClearNextGameState();
        Engine::GetGameStateManager().SetNextGameState(static_cast<int>(States::Mode1));
    }
    if (Engine::GetInput().KeyJustPressed(CS230::Input::Keys::W)) {
        Engine::GetGameStateManager().ReloadState();

    }
}

void Mode2::Draw() {
    GetGSComponent<Background>()->Draw(*GetGSComponent<CS230::Cam>());

    GetGSComponent<CS230::GameObjectManager>()->DrawAll();
}

void Mode2::Unload() {
    ship_ptr = nullptr;
    GetGSComponent<CS230::GameObjectManager>()->Unload();
    GetGSComponent<Background>()->Unload();
    ClearGSComponents();
}