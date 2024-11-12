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
#include "../Engine/Rapidjson.h"
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
#include "Boss.h"

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
    AddGSComponent(new CS230::Map());


    //// ship
    ship_ptr = new Ship({ Engine::window_width / 2, Engine::window_height / 2 });
    GetGSComponent<CS230::GameObjectManager>()->Add(ship_ptr);

    //// camera
    camera = new CS230::Cam();
    AddGSComponent(camera);

    //// background
    background = new Background();
    AddGSComponent(background);    

    //// audio
    Mix_Music* sample = Engine::GetAudioManager().LoadMusic("assets/audios/basic_beat_100_5.wav", "sample");
    if (sample) {
        Engine::GetAudioManager().PlayMusic(sample, -1);
    }

    //// to generate fish
    fishGenerator = new FishGenerator();
    Engine::GetGameStateManager().GetGSComponent<Fish>()->ReadFishCSV("assets/scenes/Fish.csv");

    //// reef
    reef = new Reef({ -400,200 });
    GetGSComponent<CS230::GameObjectManager>()->Add(reef);

    GetGSComponent<Background>()->Add("assets/images/temp_back.png", 0.25f);

    // Mouse and Particle
    AddGSComponent(new CS230::ParticleManager<Particles::MouseFollow>());
    AddGSComponent(new Mouse());
    //Engine::GetGameStateManager().GetGSComponent<CS230::ParticleManager<Particles::Mouse>>()->Emit(2, mouse_position, { 0, 0 }, { 0, 100 }, M_PI / 2);
    Boss::LoadBossfile();
   

    // UI
    AddGSComponent(new UIManager());
    ui_manager = GetGSComponent<UIManager>();

    ui_manager->AddUI(std::make_unique<FuelUI>(ship_ptr));

    // Map
    GetGSComponent<CS230::Map>()->ParseSVG("assets/maps/test.svg");
}

void Mode1::Update(double dt) {
    UpdateGSComponents(dt);
    GetGSComponent<CS230::GameObjectManager>()->UpdateAll(dt);

    //camera postion update
    camera->SetPosition(ship_ptr->GetPosition());
    camera->Update(dt, ship_ptr->GetPosition(), ship_ptr->IsShipMoving());

	fishGenerator->GenerateFish(dt);

    if (Engine::GetInput().KeyJustPressed(CS230::Input::Keys::Q)) {
        Engine::GetGameStateManager().ClearNextGameState();
        Engine::GetGameStateManager().SetNextGameState(static_cast<int>(States::Mode2));

    }
    if (Engine::GetInput().KeyJustPressed(CS230::Input::Keys::W)) {
        Engine::GetGameStateManager().ReloadState();

    }
    if (Engine::GetInput().KeyJustPressed(CS230::Input::Keys::E)) {
        boss_ptr = new Boss({ 0,0 }, Boss::BossType::e);
        GetGSComponent<CS230::GameObjectManager>()->Add(boss_ptr);
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
    Engine::GetRender().ClearDrawCalls();
	ClearGSComponents();
    Engine::GetAudioManager().StopMusic();
	//fishGenerator->DeleteFish();
}
