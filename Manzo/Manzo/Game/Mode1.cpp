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
#include "../Engine/Particle.h"
#include "../Engine/MapManager.h"
#include "../Engine/UI.h"


#include "Particles.h"
#include "Mouse.h"
#include "States.h"
#include "Mode1.h"
#include "BeatSystem.h"
#include "Background.h"
#include "Ship.h"
#include "Fish.h"
#include "Reef.h"
#include "Skill.h"

#include <iostream>

Mode1::Mode1()
{
}


void Mode1::Load() {

#ifdef _DEBUG
    AddGSComponent(new CS230::ShowCollision());
#else
#endif
    // component
    AddGSComponent(new CS230::GameObjectManager());
    AddGSComponent(new Beat());
    AddGSComponent(new AudioManager());
    AddGSComponent(new CS230::Map());


    //// ship
    ship_ptr = new Ship({ 0, 0 });
    GetGSComponent<CS230::GameObjectManager>()->Add(ship_ptr);

    //// camera
    camera = new CS230::Cam();
    AddGSComponent(camera);
    camera->SetLimit({{0,0},{1920,1080}});

    //// background
    background = new Background();
    AddGSComponent(background);


    //// audio
    Mix_Music* sample = GetGSComponent<AudioManager>()->LoadMusic("assets/audios/basic_beat_100_5.wav", "sample");
    if (sample) {
        GetGSComponent<AudioManager>()->PlayMusic(sample, -1);
    }

    //// to generate fish
    fishGenerator = new FishGenerator();
    Engine::GetGameStateManager().GetGSComponent<Fish>()->ReadFishCSV("assets/scenes/Fish.csv");

    //// reef
    reef = new Reef({ -400,200 });
    GetGSComponent<CS230::GameObjectManager>()->Add(reef);

    GetGSComponent<Background>()->Add("assets/images/sky_first.png", 0.3f);
    GetGSComponent<Background>()->Add("assets/images/sky_second.png", 0.4f);
    GetGSComponent<Background>()->Add("assets/images/sky_third.png", 0.5f);
    GetGSComponent<Background>()->Add("assets/images/sky_fourth.png", 0.6f);
    GetGSComponent<Background>()->Add("assets/images/sky_fifth.png", 0.7f);


    // Mouse and Particle
    AddGSComponent(new CS230::ParticleManager<Particles::MouseFollow>());
    AddGSComponent(new Mouse());
    //Engine::GetGameStateManager().GetGSComponent<CS230::ParticleManager<Particles::Mouse>>()->Emit(2, mouse_position, { 0, 0 }, { 0, 100 }, M_PI / 2);

    // UI
    AddGSComponent(new UIManager());
    ui_manager = GetGSComponent<UIManager>();
    std::shared_ptr<FuelUI> fuel_ui = std::make_shared<FuelUI>(ship_ptr);
    ui_manager->AddUI(fuel_ui);

    // Map
    GetGSComponent<CS230::Map>()->ParseSVG("assets/maps/test.svg");
}

void Mode1::Update(double dt) {
    UpdateGSComponents(dt);
    GetGSComponent<CS230::GameObjectManager>()->UpdateAll(dt);

    //camera postion update
    camera->Update(dt, ship_ptr->GetPosition(), ship_ptr->IsShipMoving());

	fishGenerator->GenerateFish(dt);

    if (Engine::GetInput().KeyJustPressed(CS230::Input::Keys::Q)) {
        Engine::GetGameStateManager().ClearNextGameState();
        Engine::GetGameStateManager().SetNextGameState(static_cast<int>(States::Mode2));

    }
    if (Engine::GetInput().KeyJustPressed(CS230::Input::Keys::W)) {
        Engine::GetGameStateManager().ReloadState();

    }
}

void Mode1::Draw() {
    GetGSComponent<Background>()->Draw(*GetGSComponent<CS230::Cam>());
    GetGSComponent<CS230::GameObjectManager>()->DrawAll();
    ui_manager->AddDrawCalls();
}

void Mode1::Unload() {

    ship_ptr = nullptr;
	GetGSComponent<CS230::GameObjectManager>()->Unload();
    GetGSComponent<Background>()->Unload();
	//fishGenerator->DeleteFish();
	ClearGSComponents();
}
