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
    Engine::GetShaderManager().LoadShader("post_process", "assets/shaders/post_default.vert", "assets/shaders/post_default.frag");
    Engine::GetShaderManager().LoadShader("post_bloom", "assets/shaders/post_default.vert", "assets/shaders/post_bloom.frag");
    Engine::GetShaderManager().LoadShader("blur", "assets/shaders/default.vert", "assets/shaders/blur.frag");
    Engine::GetShaderManager().LoadShader("change_alpha", "assets/shaders/default.vert", "assets/shaders/change_alpha.frag");
    Engine::GetShaderManager().LoadShader("change_alpha_no_texture", "assets/shaders/default.vert", "assets/shaders/change_alpha_no_texture.frag");

    // component
    AddGSComponent(new CS230::GameObjectManager());
    beat_system = new Beat();
    AddGSComponent(beat_system);
    AddGSComponent(new CS230::Map());
    god_ray = new GodRay();
    AddGSComponent(god_ray);
    //AddGSComponent(new Pump());


    //Particle
    AddGSComponent(new CS230::ParticleManager<Particles::Plankton>());
    AddGSComponent(new CS230::ParticleManager<Particles::FuelBubble>());
    AddGSComponent(new CS230::ParticleManager<Particles::BubblePop>());


    //// ship
    ship_ptr = new Ship({ 0, 0 });
    GetGSComponent<CS230::GameObjectManager>()->Add(ship_ptr);

    //// camera
    Math::rect Boundary({ -BoundaryX, -BoundaryY }, { BoundaryX, BoundaryY });
    camera = new CS230::Cam();
    AddGSComponent(camera);
    camera->SetLimit(Boundary);

    //// background
    background = new Background();
    AddGSComponent(background);    

    //// to generate fish
    //fishGenerator = new FishGenerator();
    //Engine::GetGameStateManager().GetGSComponent<Fish>()->ReadFishCSV("assets/scenes/Fish.csv");

    background->Add("assets/images/background/temp_background.png", 0.0f);
    /*background->Add("assets/images/background/bg1.png", 0.3f);
    background->Add("assets/images/background/bg2.png", 0.4f);
    background->Add("assets/images/background/bg3.png", 0.5f);
    background->Add("assets/images/background/bg4.png", 0.6f);
    background->Add("assets/images/background/bg5.png", 0.7f);*/

    // Mouse and Particle
    AddGSComponent(new CS230::ParticleManager<Particles::MouseFollow>());
    Boss::LoadBossfile();
    

    // UI
    AddGSComponent(new UIManager());
    ui_manager = GetGSComponent<UIManager>();
    ui_manager->AddUI(std::make_unique<FuelUI>(ship_ptr));
    ui_manager->AddUI(std::make_unique<Mouse>());

    // Map
    GetGSComponent<CS230::Map>()->ParseSVG("assets/maps/map3.svg");
    //for (int i = 0; i < 25; i++)
    //{
    //    GetGSComponent<Boss>()->ReadBossJSON(static_cast<Boss::BossType>(i));
    //    BossFirstPos.push_back(GetGSComponent<Boss>()->GetFirstPosition());
    //}



    boss_ptr = new Boss({ 750,500 }, Boss::BossType::e);
    boss_ptr->ReadBossJSON(Boss::BossType::e);
    BossFirstPos.push_back(boss_ptr->GetFirstPosition());

    std::array<int, 2> bossFirstPos = boss_ptr->GetFirstPosition();
    vec3 bossPosition = { static_cast<float>(bossFirstPos[0])*0.2f, static_cast<float>(bossFirstPos[1]) * 0.2f, 0.0f };

    //// audio
    //Engine::GetAudioManager().LoadSound("assets/audios/back_temp.wav", "sample");
    //bossChannelID = Engine::GetAudioManager().PlaySounds("assets/audios/back_temp.wav", bossPosition, Engine::GetAudioManager().VolumeTodB(1.0f));

    // Convert Boss Position to vec3

    // Get Ship Position


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
    Engine::GetAudioManager().LoadSound("assets/audios/100BPM_edm_temp.wav");
    beat_system->LoadMusicToSync("assets/audios/100BPM_edm_temp.wav");
}

void Mode1::Update(double dt) {
    UpdateGSComponents(dt);
    GetGSComponent<CS230::GameObjectManager>()->UpdateAll(dt);
    Engine::GetGameStateManager().GetGSComponent<CS230::ParticleManager<Particles::Plankton>>()->Spray();
    //std::cout << "update: " << dt << std::endl;
    //camera postion update
    camera->Update(dt, ship_ptr->GetPosition(), ship_ptr->IsShipMoving());

    // Update Fish Generator
    //fishGenerator->GenerateFish(dt);

    // Update Skills
    skill_ptr->Update();

    // Handle Input
    if (ship_ptr->IsShipUnder() && Engine::GetInput().KeyJustPressed(CS230::Input::Keys::Q)) {
        Engine::GetGameStateManager().ClearNextGameState();
        Engine::GetGameStateManager().SetNextGameState(static_cast<int>(States::Mode2));
    }

    if (Engine::GetInput().KeyJustPressed(CS230::Input::Keys::W)) {
        Engine::GetGameStateManager().ReloadState();
    }

    if (Engine::GetInput().KeyJustPressed(CS230::Input::Keys::E) && !Isboss) {
        GetGSComponent<CS230::GameObjectManager>()->Add(boss_ptr);
        Isboss = true;
    }

    /*if (!Engine::GetAudioManager().IsPlaying(bossChannelID)) {
        std::cout << "!!!!!!!!!!!No Sound!!!!!!!!!!1111" << std::endl;
    }*/

    // Update 3D Audio

    //vec3 shipPosition = { ship_ptr->GetPosition().x, ship_ptr->GetPosition().y, 0.0f };
    //Engine::GetAudioManager().Set3dListenerAndOrientation(shipPosition, vec3{ 0.0f, 1.0f, 0.0f }, vec3{ 0.0f, 0.0f, 1.0f });
    //Engine::GetAudioManager().SetChannel3dPosition(bossChannelID, bossPosition);
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
    god_ray->Draw();
    GetGSComponent<CS230::GameObjectManager>()->DrawAll();
    ui_manager->AddDrawCalls();

    // Draw Font
    //Engine::GetFontManager().PrintText(FontType::Bold, "HI", { 0.f,0.f }, 0.0f, 0.001f, { 1.0f,1.0f,1.0f });
    //Engine::GetFontManager().PrintText(FontType::Thin, "A", { 0.5f,0.5f }, 0.0f, 0.0005f, { 0.0f,0.0f,0.0f });
}

void Mode1::Unload() {

    ship_ptr = nullptr;
    //fishGenerator->~FishGenerator();
    //delete fishGenerator;
    //fishGenerator = nullptr;
	GetGSComponent<CS230::GameObjectManager>()->Unload();
    GetGSComponent<Background>()->Unload();
    Engine::GetRender().ClearDrawCalls();
    ui_manager->UnloadUI();
	ClearGSComponents();
    Engine::GetAudioManager().StopAllChannels();
    Engine::Instance().ResetSlowDownFactor();
}
