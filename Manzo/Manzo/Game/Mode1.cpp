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

    //shader
    Engine::GetShaderManager().LoadShader("pixelate", "assets/shaders/default.vert", "assets/shaders/pixelate.frag");

    // component
    AddGSComponent(new CS230::GameObjectManager());
    beat_system = new Beat();
    AddGSComponent(beat_system);
    AddGSComponent(new CS230::Map());

    //// ship
    ship_ptr = new Ship({ 0, 0 });
    GetGSComponent<CS230::GameObjectManager>()->Add(ship_ptr);

    //// camera
    Math::rect Boundary({ -640,-360 }, { 640,360 });
    camera = new CS230::Cam();
    AddGSComponent(camera);
    camera->SetLimit(Boundary);

    //// background
    background = new Background();
    AddGSComponent(background);    

    //// to generate fish
    fishGenerator = new FishGenerator();
    Engine::GetGameStateManager().GetGSComponent<Fish>()->ReadFishCSV("assets/scenes/Fish.csv");

    background->Add("assets/images/background/temp_background.png", 0.0f);
    background->Add("assets/images/background/bg1.png", 0.3f);
    background->Add("assets/images/background/bg2.png", 0.4f);
    background->Add("assets/images/background/bg3.png", 0.5f);
    background->Add("assets/images/background/bg4.png", 0.6f);
    background->Add("assets/images/background/bg5.png", 0.7f);

    // Mouse and Particle
    AddGSComponent(new CS230::ParticleManager<Particles::MouseFollow>());
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

    //// audio
    Engine::GetAudioManager().LoadMusic("assets/audios/100BPM_edm_temp.wav", "sample");
    beat_system->LoadMusicToSync("sample");
}

void Mode1::Update(double dt) {
    UpdateGSComponents(dt);
    GetGSComponent<CS230::GameObjectManager>()->UpdateAll(dt);
    //std::cout << "update: " << dt << std::endl;
    //camera postion update
    camera->Update(dt, ship_ptr->GetPosition(), ship_ptr->IsShipMoving());

	//fishGenerator->GenerateFish(dt);
    skill_ptr->Update();
    if (ship_ptr->IsShipUnder() && Engine::GetInput().KeyJustPressed(CS230::Input::Keys::Q)) {
        Engine::GetGameStateManager().ClearNextGameState();
        Engine::GetGameStateManager().SetNextGameState(static_cast<int>(States::Mode2));
    }

    if (Engine::GetInput().KeyJustPressed(CS230::Input::Keys::W)) {
        Engine::GetGameStateManager().ReloadState();

    }
    if (Engine::GetInput().KeyJustPressed(CS230::Input::Keys::E)&&!Isboss) {
        boss_ptr = new Boss({750,500}, Boss::BossType::e);
        GetGSComponent<CS230::GameObjectManager>()->Add(boss_ptr);
        Isboss = true;
    }
}

void Mode1::FixedUpdate(double dt)
{
    //std::cout << "fixedupdate: " << dt << std::endl;
    if (GetGSComponent<CS230::GameObjectManager>()) {
        GetGSComponent<CS230::GameObjectManager>()->FixedUpdateAll(dt);
    }
}

void Mode1::Draw() {
    GetGSComponent<Background>()->Draw(*GetGSComponent<CS230::Cam>());
    GetGSComponent<CS230::Map>()->AddDrawCall();
    GetGSComponent<CS230::GameObjectManager>()->DrawAll();
    ui_manager->AddDrawCalls();

    // Draw Font
     //Engine::GetFontManager().PrintText("HI", { 0.f,0.f }, 0.0f, 0.001f, { 1.0f,1.0f,1.0f });
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
    Engine::Instance().ResetSlowDownFactor();
}
