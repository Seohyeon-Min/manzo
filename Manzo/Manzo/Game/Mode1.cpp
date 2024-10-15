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
}


void Mode1::Load() {

#ifdef _DEBUG
    AddGSComponent(new CS230::ShowCollision());
#else
#endif
    // compenent
    AddGSComponent(new CS230::GameObjectManager());
    AddGSComponent(new Beat());
    AddGSComponent(new AudioManager());

    //// ship
    ship_ptr = new Ship({ 0, 0 });
    GetGSComponent<CS230::GameObjectManager>()->Add(ship_ptr);

    //// camera
    camera = new CS230::Cam({ { -100 , -100 }, { 100, 100} });
    AddGSComponent(camera);

    //// audio
    Mix_Music* sample = GetGSComponent<AudioManager>()->LoadMusic("assets/audios/basic_beat_100_5.wav", "sample");
    if (sample) {
        GetGSComponent<AudioManager>()->PlayMusic(sample, -1);
    }

    //// to generate fish
    fishGenerator = new FishGenerator();

    //// reef
    reef = new Reef({ -400,200 });
    GetGSComponent<CS230::GameObjectManager>()->Add(reef);

    GetGSComponent<CS230::Cam>()->SetLimit({ { -200, -200 }, { 200, 200 } });
    camera->SetPosition({0,0});
}

void Mode1::Update(double dt) {
    UpdateGSComponents(dt);
    GetGSComponent<CS230::GameObjectManager>()->UpdateAll(dt);

    //camera postion update
    camera->Update(dt, ship_ptr->GetPosition());

    std::cout << "ship pos: " << ship_ptr->GetPosition().x << ", " << ship_ptr->GetPosition().y << std::endl;
    std::cout << "camera pos: " << camera->GetPosition().x << ", " << camera->GetPosition().y << std::endl;

	fishGenerator->GenerateFish(dt);
}

void Mode1::Draw() {
    GetGSComponent<CS230::GameObjectManager>()->DrawAll();
}

void Mode1::Unload() {

	GetGSComponent<CS230::GameObjectManager>()->Unload();
	fishGenerator->DeleteFish();
	ClearGSComponents();
}
