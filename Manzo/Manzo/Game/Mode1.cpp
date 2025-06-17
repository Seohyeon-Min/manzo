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
#include "Background.h"
#include "Ship.h"
#include "Fish.h"
#include "Boss.h"
#include "Monster.h"

#include <utility>
#include <iostream>

Mode1::Mode1()
{
}

void Mode1::Load()
{

#ifdef _DEBUG
	AddGSComponent(new ShowCollision());
#else
#endif

	// audio
	Engine::GetAudioManager().LoadMusic("assets/audios/Level1_bgm.mp3", "Level1_bgm", false);
	Engine::GetAudioManager().LoadMusic("assets/audios/morse/e.mp3", "e morse", true);
	Engine::GetAudioManager().LoadMusic("assets/audios/Dash.mp3", "dash", false, false);
	Engine::GetAudioManager().Set3DMode(FMOD_3D_LINEARROLLOFF);

	// component
	AddGSComponent(new GameObjectManager());
	beat_system = &Engine::GetBeat();
	beat_system->LoadMusicToSync("Level1_bgm");

	// Particle
	AddGSComponent(new ParticleManager<Particles::Plankton>());
	AddGSComponent(new ParticleManager<Particles::FuelBubble>());
	AddGSComponent(new ParticleManager<Particles::BubblePop>());
	AddGSComponent(new ParticleManager<Particles::HitPraticle>());
	AddGSComponent(new ParticleManager<Particles::HitPraticle2>());
	AddGSComponent(new ParticleManager<Particles::CaptureEffect>());
	AddGSComponent(new ParticleManager<Particles::bossEbulletParticle>());
	AddGSComponent(new ParticleManager<Particles::bossEKiraKiraParticle>());
	AddGSComponent(new ParticleManager<Particles::bossEParticle1>());
	AddGSComponent(new ParticleManager<Particles::bossEParticle2>());
	AddGSComponent(new ParticleManager<Particles::bossEParticle3>());
	AddGSComponent(new ParticleManager<Particles::BossBlackCircleParticle>());

	// get cali
	double cali = Engine::GetSaveDataManager().GetSaveData().user_calibration;
	Engine::GetBeat().SetUserCali(cali);
	std::cout << cali << std::endl;

	//// camera
	vec2 start_position = { 600, -500 };
	//vec2 start_position = {404, -6411};
	Math::rect cam_limit = Math::rect({-1200, -500}, {4300, -12000});
	camera = new Cam();
	camera->SetPosition(start_position);
	camera->SetLimit(cam_limit);
	AddGSComponent(camera);

	//// ship
	ship_ptr = new Ship(start_position);
	GetGSComponent<GameObjectManager>()->Add(ship_ptr);

	//// background
	background = new Background();
	AddGSComponent(background);

	//// to generate fish
	fishGenerator = new FishGenerator();
	AddGSComponent(fishGenerator);
	fishGenerator->ReadFishCSV("assets/images/fish/Fish.csv");

	// background
	background->Add("assets/images/background/rock_pillar/r1.png", {350.f,-1600.f}, 0.1f);
	background->Add("assets/images/background/rock_pillar/r2.png", { 5050.f,-900.f }, 0.1f);
	background->Add("assets/images/background/rock_pillar/r3.png", { 2400.f,-3500.f }, 0.05f);
	background->Add("assets/images/background/rock_pillar/r4.png", { 3900.f,-2550.f }, 0.05f);
	background->Add("assets/images/background/rock_pillar/r5.png", { 1500.f,-3600.f }, 0.1f);
	background->Add("assets/images/background/rock_pillar/r6.png", { 5420.f,-370.f }, 0.2f);
	background->Add("assets/images/background/rock_pillar/r8.png", { 4300.f,-3700.f }, 0.13f);

	// Map
	AddGSComponent(new MapManager());
	GetGSComponent<MapManager>()->AddMapFile("assets/maps/level1.svg"); 
	GetGSComponent<MapManager>()->AddMapFile("assets/maps/level4.svg");
	GetGSComponent<MapManager>()->LoadFirstMap();
	


	// Boss
	Boss::LoadBossfile();
	// for (int i = 0; i < 25; i++)
	// for (int i = 0; i < 25; i++)
	//{
	//     GetGSComponent<Boss>()->ReadBossJSON(static_cast<Boss::BossType>(i));
	//     BossFirstPos.push_back(GetGSComponent<Boss>()->GetFirstPosition());
	// }
	boss_ptr_e = new Boss({4100, -5300}, Boss::BossName::e, Boss::BossType::MovingToLocation);
	boss_ptr_e->ReadBossJSON(Boss::BossName::e);
	BossFirstPos_e.push_back(std::make_pair(boss_ptr_e->GetFirstPosition()[0], boss_ptr_e->GetFirstPosition()[1]));
	bossPosition_e = {4100, -5300, 0.0f};

	boss_ptr_y = new Boss({ -1200, -9873 }, Boss::BossName::y, Boss::BossType::MovingToLocationPlus);
	boss_ptr_y->ReadBossJSON(Boss::BossName::y);
	BossFirstPos_y.push_back(std::make_pair(boss_ptr_y->GetFirstPosition()[0], boss_ptr_y->GetFirstPosition()[1]));
	bossPosition_y = { -1200, -9873, 0.0f };


	// UI
	GetGSComponent<GameObjectManager>()->Add(new Mouse);
	GetGSComponent<GameObjectManager>()->Add(new FuelUI(ship_ptr));

	// monster
	GetGSComponent<GameObjectManager>()->Add(new Monster(ship_ptr, { 2200, -2000 }));

	// Module
	auto& saveData = Engine::GetSaveDataManager().GetSaveData();
	firstBuy = saveData.module1.buy;
	secondBuy = saveData.module2.buy;

	module = new Module({ 0, 0 });
	GetGSComponent<GameObjectManager>()->Add(module);

	//Engine::GetLogger().LoadSaveFile();

	// fish collection
	fishCollection = saveData.fishCollection;

	// money
	fishGenerator->SetMoney(saveData.money);

	// module1
	if (saveData.module1.set) {
		module->SetFirstModule(true);
		GetGSComponent<GameObjectManager>()->Add(new FirstModule(ship_ptr));
	}

	// module2
	if (saveData.module2.set) {
		module->SetSecondModule(true);
		GetGSComponent<GameObjectManager>()->Add(new SecondModule(ship_ptr));
	}

	camera->SetPosition(ship_ptr->GetPosition());

	// Boss Trigger
	auto bossPosCopy = bossPosition_e;
	auto bossCopy = boss_ptr_e;
	auto shipCopy = ship_ptr;

	/*Engine::GetEventManager().AddEvent(Event("Boss E Trigger",
		[&]() { if (ship_ptr) return ship_ptr->GetPosition().y <= bossPosition.y + 300;
	return false;
		},
		[&]() {
			GetGSComponent<GameObjectManager>()->Add(boss_ptr);
			Isboss = true;
		}
	));*/

	option = new GameOption({ 0,0 });
	GetGSComponent<GameObjectManager>()->Add(option);
	raycasting = new Raycasting(ship_ptr);
}

void Mode1::Update(double dt)
{
	// audio play


	//Map
	if (!GetGSComponent<MapManager>()->GetCurrentMap()->IsLevelLoaded()) {
		GetGSComponent<MapManager>()->GetCurrentMap()->ParseSVG();
	}
	else {
		//GetGSComponent<MapManager>()->LoadNextMap();
	}


	UpdateGSComponents(dt);
	GetGSComponent<GameObjectManager>()->UpdateAll(dt);
	Engine::GetGameStateManager().GetGSComponent<ParticleManager<Particles::Plankton>>()->Spray();


	// Handle Input
	if (Engine::GetInput().KeyJustPressed(Input::Keys::TAB) && ship_ptr->GetPosition().y >= -800.f)
	{
		Engine::GetGameStateManager().ClearNextGameState();
		Engine::GetGameStateManager().SetNextGameState(static_cast<int>(States::Mode2));
	}

	if (Engine::GetInput().KeyJustPressed(Input::Keys::E) && !Isboss)
	{
		GetGSComponent<GameObjectManager>()->Add(boss_ptr_y);
		Isboss = true;
	}

	//if (Engine::GetInput().KeyJustPressed(Input::Keys::E) && !Isboss)
	//{
	//	GetGSComponent<GameObjectManager>()->Add(boss_ptr_e);
	//	Isboss = true;
	//}
#ifdef _DEBUG
	AddGSComponent(new ShowCollision());

	if (Engine::GetInput().KeyJustPressed(Input::Keys::W))
	{
		Engine::GetGameStateManager().ReloadState();
	}


#else
#endif

	if (!option->isOpened())
	{

		Engine::GetGameStateManager().GetGSComponent<ParticleManager<Particles::Plankton>>()->Spray();


		beat_system->Update(dt);

		// Handle Input
		if (Engine::GetInput().KeyJustPressed(Input::Keys::TAB) && ship_ptr->GetPosition().y >= -800.f)
		{
			Engine::GetGameStateManager().ClearNextGameState();
			Engine::GetGameStateManager().SetNextGameState(static_cast<int>(States::Mode2));
		}

		if (Engine::GetInput().KeyJustPressed(Input::Keys::V))
		{
			Engine::GetGameStateManager().ClearNextGameState();
			Engine::GetGameStateManager().SetNextGameState(static_cast<int>(States::Tutorial));
		}

		if (Isboss)
		{		
			camera->SetSmoothPosition(boss_ptr_y->GetPosition());
			camera->Update(dt, { boss_ptr_y->GetPosition().x,boss_ptr_y->GetPosition().y - 300 }, ship_ptr->IsShipMoving());

		}
		else{
		// camera postion update
			camera->Update(dt, ship_ptr->GetPosition(), ship_ptr->IsShipMoving());
		}

		// Update Fish Generator
		fishGenerator->GenerateFish(dt);

		// Update 3D Audio with smooth transition for ship position
		smoothShipPosition.x = std::lerp(previousPosition.x, ship_ptr->GetPosition().x, 0.1f);
		smoothShipPosition.y = std::lerp(previousPosition.y, ship_ptr->GetPosition().y, 0.1f);
		previousPosition = smoothShipPosition;

		// Calculate the distance between ship and boss positions
		float dx = smoothShipPosition.x - bossPosition_e.x;
		float dy = smoothShipPosition.y - bossPosition_e.y;
		float distance = std::sqrt(dx * dx + dy * dy);

		// Check if within the max distance and apply 3D audio accordingly
		bool isWithinRange = distance < maxDistance;

		Engine::GetAudioManager().Set3dListenerAndOrientation(smoothShipPosition, vec3{ 0.0f, -1.0f, 0.0f }, vec3{ 0.0f, 0.0f, 1.0f });

		// Apply 3D position for the boss and calculate volume based on the distance
		if (isWithinRange && !Isboss)
		{
			if (!soundPlaying)
			{
				Engine::GetAudioManager().PlayMusics("e morse");
				soundPlaying = true;
			}
			else
			{
				if (!replay)
				{
					Engine::GetAudioManager().RestartPlayMusic("e morse");
					replay = true;
				}
			}
			Engine::GetAudioManager().SetChannel3dPosition("e morse", bossPosition_e);

			float volumeFactor = 1.0f - std::clamp(distance / 300.0f, 0.0f, 1.0f);
			// float volume = std::lerp(0.0f, 1.0f, volumeFactor);
			float volume = std::lerp(-20.0f, 1.0f, volumeFactor);

			Engine::GetAudioManager().SetChannelVolume("e morse", volume);
		}
		else
		{
			Engine::GetAudioManager().StopPlayingMusic("e morse");
			replay = false;
		}
	}
	else
	{
		Engine::GetAudioManager().StopPlayingMusic("e morse");
		Engine::GetAudioManager().StopPlayingMusic("Level1_bgm");
		Engine::GetAudioManager().StopPlayingMusic("dash");

		soundPlaying = false;
		replay = false;
		beat_system->SetPause(true);
	}
}

void Mode1::FixedUpdate(double dt)
{
	// std::cout << "fixedupdate: " << dt << std::endl;
	if (GetGSComponent<GameObjectManager>())
	{
		GetGSComponent<GameObjectManager>()->FixedUpdateAll(dt);
	}
}

void Mode1::Draw()
{
	background->ShaderBackgroundDraw(Engine::GetShaderManager().GetShader("sea_background"), *GetGSComponent<Cam>(), ship_ptr);
	GetGSComponent<Background>()->Draw(*GetGSComponent<Cam>());
	GetGSComponent<GameObjectManager>()->DrawAll();

	raycasting->Render();

	if (ship_ptr->GetFuel() <= 0)
	{
		Engine::GetFontManager().PrintText(FontType::AlumniSans_Medium, FontAlignment::LEFT, "CLICK TO RESTART", { 0, 0 }, 10.098f, { 1.f, 1.f, 1.f }, 1.0f);
		// Draw Font
	}

	if (module->IsSecondSetted())
	{
		Engine::GetFontManager().PrintText(FontType::AlumniSans_Medium, FontAlignment::LEFT, std::to_string(static_cast<int>(ship_ptr->GetFuel())), { -0.001f, 0.75f }, 0.05f, { 1.f, 1.f, 1.f }, 1.f, false);
	}
}

void Mode1::Unload()
{

	//Engine::GetLogger().WriteSaveFile(fishCollection, GetGSComponent<Fish>()->GetMoney(), firstBuy, module->IsFirstSetted(), Engine::GetLogger().GetModule1XPos(), secondBuy, module->IsSecondSetted(), Engine::GetLogger().GetModule2XPos());
	SaveData save = Engine::GetSaveDataManager().GetSaveData();
	GetGSComponent<MapManager>()->GetMap(0)->UnloadAll();

	ModuleData m1{ firstBuy, module->IsFirstSetted(), save.module1.pos };
	ModuleData m2{ secondBuy, module->IsSecondSetted(), save.module2.pos };

	Engine::GetSaveDataManager().SetFishData(
		fishGenerator->GetMoney(),
		fishCollection
	);

	Engine::GetSaveDataManager().SetModuleData(m1, m2);
	Engine::GetSaveDataManager().Save();
	//Engine::GetSaveDataManager().UpdateSaveData(save);

	ship_ptr = nullptr;
	playing = false;
	soundPlaying = false;
	replay = false;
	Isboss = false;
	beat_system->CleartoOriginal();

	GetGSComponent<GameObjectManager>()->Unload();
	GetGSComponent<Background>()->Unload();
	Engine::GetRender().ClearDrawCalls();
	ClearGSComponents();
	Engine::GetAudioManager().StopAllChannels();
	Engine::Instance()->ResetSlowDownFactor();
}