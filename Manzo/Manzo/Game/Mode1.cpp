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


#include <utility>
#include <iostream>

Mode1::Mode1()
{
}


void Mode1::Load() {

#ifdef _DEBUG
    AddGSComponent(new ShowCollision());
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
    AddGSComponent(new GameObjectManager());
    beat_system = new Beat();
    AddGSComponent(beat_system);
	beat_system->LoadMusicToSync("assets/audios/bgm_original.wav");///////////////////////

    AddGSComponent(new Map());
    god_ray = new GodRay();
    AddGSComponent(god_ray);
    //AddGSComponent(new Pump());


    // Mouse and Particle
    AddGSComponent(new ParticleManager<Particles::MouseFollow>()); // wait, are we using it?
    AddGSComponent(new ParticleManager<Particles::Plankton>());
    AddGSComponent(new ParticleManager<Particles::FuelBubble>());
    AddGSComponent(new ParticleManager<Particles::BubblePop>());
    AddGSComponent(new ParticleManager<Particles::HitEffect>());
    AddGSComponent(new ParticleManager<Particles::HitEffect2>());
    AddGSComponent(new ParticleManager<Particles::CaptureEffect>());

    //// camera
    Math::rect Boundary({ -640,-360 }, { 640,360 });
    camera = new Cam();
    AddGSComponent(camera);
    camera->SetLimit(Boundary);

    //// ship
    ship_ptr = new Ship({ 0, 0 });
    GetGSComponent<GameObjectManager>()->Add(ship_ptr);


	//// ship
	ship_ptr = new Ship({ 0, 0 });
	GetGSComponent<GameObjectManager>()->Add(ship_ptr);

	//// background
	background = new Background();
	AddGSComponent(background);

	//// to generate fish
	fishGenerator = new FishGenerator();
	Engine::GetGameStateManager().GetGSComponent<Fish>()->ReadFishCSV("assets/scenes/Fish.csv");

	//background
	background->Add("assets/images/background/temp_background.png", 0.0f);
	background->Add("assets/images/background/bg1.png", 0.1f);
	background->Add("assets/images/background/bg2.png", 0.12f);
	background->Add("assets/images/background/bg3.png", 0.14f);
	background->Add("assets/images/background/bg4.png", 0.16f);
	background->Add("assets/images/background/bg5.png", 0.18f);

    // UI
    AddGSComponent(new UIManager());
    ui_manager = GetGSComponent<UIManager>();
    ui_manager->AddUI(std::make_unique<FuelUI>(ship_ptr));
    ui_manager->AddUI(std::make_unique<Mouse>());

    // Map
    GetGSComponent<Map>()->ParseSVG("assets/maps/test2.svg");

	Engine::GetAudioManager().LoadMusic("assets/audios/bgm_original.wav",false);
	Engine::GetAudioManager().LoadMusic("assets/audios/e.wav", true);
	Engine::GetAudioManager().Set3DMode(FMOD_3D_LINEARROLLOFF);

	//Boss
	Boss::LoadBossfile();
	//for (int i = 0; i < 25; i++)
    //{
    //    GetGSComponent<Boss>()->ReadBossJSON(static_cast<Boss::BossType>(i));
    //    BossFirstPos.push_back(GetGSComponent<Boss>()->GetFirstPosition());
    //}
	boss_ptr = new Boss({ 750,500 }, Boss::BossName::e, Boss::BossType::MovingToLocation);
	boss_ptr->ReadBossJSON(Boss::BossName::e);
	BossFirstPos.push_back(std::make_pair(boss_ptr->GetFirstPosition()[0], boss_ptr->GetFirstPosition()[1]));
	bossPosition = { static_cast<float>(BossFirstPos[0].first), static_cast<float>(BossFirstPos[0].second), 0.0f };


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
	//audio play
	if (!playing)
	{
		Engine::GetAudioManager().PlayMusics("assets/audios/bgm_original.wav");
		playing = true;
	}

	UpdateGSComponents(dt);
	GetGSComponent<GameObjectManager>()->UpdateAll(dt);
	Engine::GetGameStateManager().GetGSComponent<ParticleManager<Particles::Plankton>>()->Spray();


    // Handle Input
    if (ship_ptr->IsShipUnder() && Engine::GetInput().KeyJustPressed(Input::Keys::Q)) {
        Engine::GetGameStateManager().ClearNextGameState();
        Engine::GetGameStateManager().SetNextGameState(static_cast<int>(States::Mode2));
    }

    if (Engine::GetInput().KeyJustPressed(Input::Keys::W)) {
        Engine::GetGameStateManager().ReloadState();
    }

    if (Engine::GetInput().KeyJustPressed(Input::Keys::E) && !Isboss) {
        GetGSComponent<GameObjectManager>()->Add(boss_ptr);
        Isboss = true;
    }

	//camera postion update
	camera->Update(dt, ship_ptr->GetPosition(), ship_ptr->IsShipMoving());
	camera->GetCamera().UpdateShake((float)dt);

	// Update Fish Generator
	fishGenerator->GenerateFish(dt);

	// Update Skills
	skill_ptr->Update();


	// Update 3D Audio with smooth transition for ship position
	smoothShipPosition.x = std::lerp(previousPosition.x, ship_ptr->GetPosition().x, 0.1f);
	smoothShipPosition.y = std::lerp(previousPosition.y, ship_ptr->GetPosition().y, 0.1f);
	previousPosition = smoothShipPosition;

	// Calculate the distance between ship and boss positions
	float dx = smoothShipPosition.x - bossPosition.x;
	float dy = smoothShipPosition.y - bossPosition.y;
	float distance = std::sqrt(dx * dx + dy * dy);

	// Check if within the max distance and apply 3D audio accordingly
	bool isWithinRange = distance < maxDistance;

	Engine::GetAudioManager().Set3dListenerAndOrientation(smoothShipPosition,vec3{ 0.0f, -1.0f, 0.0f },vec3{ 0.0f, 0.0f, 1.0f }	);

	// Apply 3D position for the boss and calculate volume based on the distance
	if (isWithinRange) {
		if (!soundPlaying)
		{
			Engine::GetAudioManager().PlayMusics("assets/audios/e.wav");
			soundPlaying = true;
		}
		else
		{
			if(!replay)
			{
				Engine::GetAudioManager().RestartPlayMusic(1);
				replay = true;
			}
		}
		Engine::GetAudioManager().SetChannel3dPosition(1, bossPosition);

		// Calculate the volume based on the distance
		float volumeFactor = 1.0f - std::clamp(distance / 300.0f, 0.0f, 1.0f);
		float volume = std::lerp(0.0f, 1.0f, volumeFactor);
		Engine::GetAudioManager().SetChannelVolume(0, volume);
	}
	else
	{
		Engine::GetAudioManager().StopPlayingMusic(1);
		replay = false;
	}
}

void Mode1::FixedUpdate(double dt)
{
    //std::cout << "fixedupdate: " << dt << std::endl;
    if (GetGSComponent<GameObjectManager>()) {
        GetGSComponent<GameObjectManager>()->FixedUpdateAll(dt);
    }
}

void Mode1::Draw() {
    GetGSComponent<Background>()->Draw(*GetGSComponent<Cam>());
    GetGSComponent<Map>()->AddDrawCall();
    god_ray->Draw();
    GetGSComponent<GameObjectManager>()->DrawAll();
    ui_manager->AddDrawCalls();

    // Draw Font
    //Engine::GetFontManager().PrintText(FontType::Bold, "HI", { 0.f,0.f }, 0.0f, 0.001f, { 1.0f,1.0f,1.0f });
    //Engine::GetFontManager().PrintText(FontType::Thin, "A", { 0.5f,0.5f }, 0.0f, 0.0005f, { 0.0f,0.0f,0.0f });
}

void Mode1::Unload() {

	ship_ptr = nullptr;
	fishGenerator->~FishGenerator();
	delete fishGenerator;
	playing = false;
	fishGenerator = nullptr;
	GetGSComponent<GameObjectManager>()->Unload();
	GetGSComponent<Background>()->Unload();
	Engine::GetRender().ClearDrawCalls();
	ui_manager->UnloadUI();
	ClearGSComponents();
	Engine::GetAudioManager().StopAllChannels();
	Engine::Instance().ResetSlowDownFactor();
}