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
    ship_ptr = new Ship({ 0, 0 });
    GetGSComponent<CS230::GameObjectManager>()->Add(ship_ptr);

    //// camera
    Math::rect Boundary({ -640,-370 }, { 640,360 });
    camera = new CS230::Cam();
    AddGSComponent(camera);
    camera->SetLimit(Boundary);

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


    background->Add("assets/images/background/temp_background.png", 0.0f);
    background->Add("assets/images/background/bg1.png", 0.3f);
    background->Add("assets/images/background/bg2.png", 0.4f);
    background->Add("assets/images/background/bg3.png", 0.5f);
    background->Add("assets/images/background/bg4.png", 0.6f);
    background->Add("assets/images/background/bg5.png", 0.7f);
    /*
    GetGSComponent<Background>()->Add("assets/images/background/bg_rock4.png", 0.4f);
    GetGSComponent<Background>()->Add("assets/images/background/bg_rock2.png", 0.5f);
    GetGSComponent<Background>()->Add("assets/images/background/bg_rock1.png", 0.7f);*/


    //testing fish
    /*BackgroundFish* bg_fish = new BackgroundFish();
    GetGSComponent<CS230::GameObjectManager>()->Add(bg_fish);
    bg_fish->AddBackgroundFishes(bg_fish);*/

    // Mouse and Particle
    AddGSComponent(new CS230::ParticleManager<Particles::MouseFollow>());
    //AddGSComponent(new Mouse());
    //Engine::GetGameStateManager().GetGSComponent<CS230::ParticleManager<Particles::Mouse>>()->Emit(2, mouse_position, { 0, 0 }, { 0, 100 }, M_PI / 2);
    Boss::LoadBossfile();
   

    // UI
    AddGSComponent(new UIManager());
    ui_manager = GetGSComponent<UIManager>();
    ui_manager->AddUI(std::make_unique<FuelUI>(ship_ptr));
    ui_manager->AddUI(std::make_unique<Mouse>());

    // Map
    GetGSComponent<CS230::Map>()->ParseSVG("assets/maps/test2.svg");

    // Skill
    if (!Engine::Instance().GetTmpPtr())
    {
        Engine::Instance().SetTmpPtr(new Skillsys);
        skill_ptr = static_cast<Skillsys*>(Engine::Instance().GetTmpPtr());
        skill_ptr->SetShipPtr(ship_ptr);
    }
    else
    {
        skill_ptr = static_cast<Skillsys*>(Engine::Instance().GetTmpPtr());
        skill_ptr->SetShipPtr(ship_ptr);
    }
}

void Mode1::Update(double dt) {
    UpdateGSComponents(dt);
    GetGSComponent<CS230::GameObjectManager>()->UpdateAll(dt);

    //camera postion update
    camera->Update(dt, ship_ptr->GetPosition(), ship_ptr->IsShipMoving());

	fishGenerator->GenerateFish(dt);
    skill_ptr->Update();

    if (ship_ptr->IsShipUnder() && Engine::GetInput().KeyJustPressed(CS230::Input::Keys::Q)) {
        Engine::GetGameStateManager().ClearNextGameState();
        Engine::GetGameStateManager().SetNextGameState(static_cast<int>(States::Mode2));
    }

    //if (Engine::GetGameStateManager().GetGSComponent<Fish>()->GetMoney() >= 5) {
    //        Engine::GetGameStateManager().ClearNextGameState();
    //        Engine::GetGameStateManager().SetNextGameState(static_cast<int>(States::Clear));
    //}
    //if (ship_ptr->IsFuelZero()) {
    //    Engine::GetGameStateManager().ClearNextGameState();
    //    Engine::GetGameStateManager().SetNextGameState(static_cast<int>(States::GameOver));
    //}
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
    //GetGSComponent<Mouse>()->AddDrawCall();
    GetGSComponent<CS230::Map>()->AddDrawCall();
    GetGSComponent<CS230::GameObjectManager>()->DrawAll();
    ui_manager->AddDrawCalls();
}

void Mode1::Unload() {

    ship_ptr = nullptr;
    fishGenerator->~FishGenerator();
    delete fishGenerator;
    fishGenerator = nullptr;
	GetGSComponent<CS230::GameObjectManager>()->Unload();
    GetGSComponent<Background>()->Unload();
    Engine::GetRender().ClearDrawCalls();
	ClearGSComponents();
    Engine::GetAudioManager().StopMusic();
	//fishGenerator->DeleteFish();
}
