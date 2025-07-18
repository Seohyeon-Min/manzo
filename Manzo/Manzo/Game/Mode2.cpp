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
#include "../Engine/Icon.h"
#include "../Engine/Render.h"

#include <cmath>

#include "States.h"
#include "Background.h"
#include "Ship.h"
#include "Mode2.h"
#include "Mouse.h"
#include "DialogBox.h"
#include "WaterRippleEffect.h"
#include "Mode3.h"
#include "Effect.h"

#include <iostream>     // for debug
#include "Module.h"

Mode2::Mode2() {}

int dialog_test_int = 0;

void Mode2::Load() {

#ifdef _DEBUG
	AddGSComponent(new ShowCollision());
#else
#endif

	// audio
	Engine::GetAudioManager().LoadMusic("assets/audios/home1.mp3", "home_intro", false, false);
	Engine::GetAudioManager().LoadMusic("assets/audios/home2.mp3", "home_replay", false);
	Engine::GetAudioManager().LoadMusic("assets/audios/Walk.mp3", "walk", false);

	// compenent
	AddGSComponent(new GameObjectManager());

	fishGenerator = new FishGenerator();
	fishGenerator->ReadFishCSV("assets/images/fish/Fish.csv");


	// player
	player_ptr = new Player({ 0, -70 });
	GetGSComponent<GameObjectManager>()->Add(player_ptr);

	// water ripple
	GetGSComponent<GameObjectManager>()->Add(new WaterRipple);

	// camera
	AddGSComponent(new Cam());
	GetGSComponent<Cam>()->SetPosition({ 0, 0 });

	// background
	background = new Background();
	AddGSComponent(background);
	background->Add("assets/images/background/house.png", 0.25f);

	// Icon
	Engine::GetIconManager().LoadIconList();

	// Dialog
	dialog_ptr = std::make_shared<Dialog>(vec2(0, 0));
	GetGSComponent<GameObjectManager>()->Add(dialog_ptr.get());
	Engine::GetDialogSystem().SetDialog(dialog_ptr);
	//Engine::GetLogger().LoadSaveFile();

	//ScenarioComponent
	//scenario = new ScenarioComponent(dialog_ptr);
	//AddGSComponent(scenario);
	//scenario->Load();

	// Module
	module_ptr = new Module({ 0, 0 });
	GetGSComponent<GameObjectManager>()->Add(module_ptr);

	today_fish_popup = new PopUp({ -420,295 }, "assets/images/special_fish_popup.spt", true);
	GetGSComponent<GameObjectManager>()->Add(today_fish_popup);

	// Inven
	inven_ptr = new Inven({ 350,0 });
	GetGSComponent<GameObjectManager>()->Add(inven_ptr);
	fishCollection = Engine::GetSaveDataManager().GetFishCollection();

	shop_ptr = new Shop({ -350, 0 });
	GetGSComponent<GameObjectManager>()->Add(shop_ptr);

	// Mouse
	GetGSComponent<GameObjectManager>()->Add(new Mouse);

	// Scenario
	Engine::GetScenarioSystem().LoadMode2Scenarios();

	//effect
	Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->Add(new Flash(1.3f));

	sell_popup = new PopUp({ 0,0 }, "assets/images/sell_popup.spt");
	GetGSComponent<GameObjectManager>()->Add(sell_popup);

	option = new GameOption({ 0,100 });
	GetGSComponent<GameObjectManager>()->Add(option);

	Engine::GetIconManager().AddIcon("Mode2_None", "can_go_shop", "computer", { 288,-40.5 }, 2.0f, false, false, true, true);
	Engine::GetIconManager().AddIcon("Mode2_None", "can_go_sea", "ship", { 0,-250 }, 1.0f, false, false, true, true);
	Engine::GetIconManager().AddIcon("FishPopUp", "close_fishPopUp", "close_icon", { 78,105 }, 1.f, false, false, true, false);
	Engine::GetIconManager().AddIcon("Option", "cali", "cali", { -160,100 }, 1.0f, false, false, true, false);
	Engine::GetIconManager().AddIcon("Option", "tutorial", "tutorial", { 160,100 }, 1.f, false, false, true, false);

	for (auto icon : Engine::GetIconManager().GetIconList())
	{
		GetGSComponent<GameObjectManager>()->Add(icon);
	}

}

void Mode2::Update(double dt) {

#ifdef _DEBUG
	if (Engine::GetInput().KeyJustPressed(Input::Keys::Q)) {
		Engine::GetGameStateManager().ClearNextGameState();
		Engine::GetGameStateManager().SetNextGameState(static_cast<int>(States::Mode1));
	}
	if (Engine::GetInput().KeyJustPressed(Input::Keys::W)) {
		Engine::GetGameStateManager().ReloadState();
	}
	if (Engine::GetInput().KeyJustPressed(Input::Keys::Space) && !isLoaded) {
		Engine::GetGameStateManager().ClearNextGameState();
		Engine::GetGameStateManager().SetNextGameState(static_cast<int>(States::Mode3));
	}

	if (Engine::GetInput().KeyJustPressed(Input::Keys::Z) && !isLoaded) {
		dialog_ptr->LoadDialogGroup("dialog-1", 0.05);
		isLoaded = true;
	}
	if ((Engine::GetInput().KeyJustPressed(Input::Keys::X) && !isLoaded)) {

		dialog_ptr->LoadRandomDialog("dialog-2", 0.05);
		isLoaded = true;
	}
#else
#endif

	UpdateGSComponents(dt);
	GetGSComponent<GameObjectManager>()->UpdateAll(dt);


	if (!option->isOpened())
	{
		//audio play
		if (!playing)
		{
			Engine::GetAudioManager().PlayMusics("home_intro");
			playing = true;
		}

		if (!playing_replay)
		{
			if (!Engine::GetAudioManager().IsPlayingMusic("home_intro"))
			{
				playing_replay = true;
				Engine::GetAudioManager().PlayMusics("home_replay");
			}
		}

		GetGSComponent<Cam>()->Update(dt, {}, false);

		////float moving~
		time += float(dt);
		//ship_ptr->SetVelocity({ 0, -(y_limit * frequency * std::cos(frequency * float(time))) });


		//Dialog
		if (Engine::GetInput().KeyJustPressed(Input::Keys::Space)) {
			dialog_ptr->NextLine();
		}


		if (Engine::GetInput().KeyJustPressed(Input::Keys::C)) {
			dialog_ptr->Hide();
			isLoaded = false;
		}

		dialog_ptr->Update(dt);

		Icon* icon = Engine::GetIconManager().GetCollidingIconWithMouse({ Engine::GetInput().GetMousePos().mouseCamSpaceX ,Engine::GetInput().GetMousePos().mouseCamSpaceY });
		bool clicked = Engine::GetInput().MouseButtonJustPressed(SDL_BUTTON_LEFT);

		if (icon != nullptr) {
			if ((icon->GetId() == "can_go_sea") && clicked && !inven_ptr->GetIsOpened()) {
				Engine::GetGameStateManager().ClearNextGameState();
				Engine::GetGameStateManager().SetNextGameState(static_cast<int>(States::Mode1));
			}
			else if ([&]() {
				std::string alias = icon->GetId();
				if (alias.size() < 5) return false;
				if (alias.substr(0, 4) != "fish") return false;
				char num = alias[4];
				n = num - '0';
				return (num >= '1' && num <= '7' && icon->IsCollidingWith(shop_ptr));
				}()) {
				flag = true;
			}
			else if ((icon->GetId() == "close_fishPopUp") && clicked)
			{
				Engine::GetIconManager().HideIconByGroup("FishPopUp");
				Engine::GetIconManager().HideIconByGroup("FishPopping");
				sell_popup->SetPop(false);
				inven_ptr->SetHowMuchSold(1);
			}
			else if ((icon->GetId() == "sell_fish") && fishCollection[n - 1] != 0 && clicked)
			{
				if (n - 1 != inven_ptr->GetTodayFishIndex())
				{
					inven_ptr->SetMoney(inven_ptr->GetMoney() + (fishGenerator->ReturnFishMoney(n) * inven_ptr->HowMuchSold()));
					inven_ptr->fishCollection[n - 1] -= inven_ptr->HowMuchSold();
				}
				else
				{
					inven_ptr->SetMoney(inven_ptr->GetMoney() + (inven_ptr->TodayFishPrice() * inven_ptr->HowMuchSold()));
					inven_ptr->fishCollection[n - 1] -= inven_ptr->HowMuchSold();
				}

				inven_ptr->SetHowMuchSold(1);
				Engine::GetIconManager().HideIconByGroup("FishPopUp");
				Engine::GetIconManager().HideIconByGroup("FishPopping");
				sell_popup->SetPop(false);
			}
		}

		// Open Inven
		if (inven_ptr->Open())
		{
			inven_ptr->SetIsOpened(true);
			Engine::GetIconManager().HideIconByGroup("Mode2_None");
			Engine::GetIconManager().ShowIconByGroup("OpenInven");
		}

		if (inven_ptr->GetIsOpened() && Engine::GetInput().KeyJustReleased(Input::Keys::X) || !inven_ptr->GetIsOpened())
		{
			inven_ptr->SetIsOpened(false);
			Engine::GetIconManager().ShowIconByGroup("Mode2_None");
			Engine::GetIconManager().HideIconByGroup("OpenInven");
		}
	}
	else
	{
		Engine::GetIconManager().HideIconById("can_go_shop");
		Engine::GetIconManager().HideIconById("module_set1");

	Engine::GetScenarioSystem().Mode2Update();

		Engine::GetAudioManager().StopPlayingMusic("home_intro");
		Engine::GetAudioManager().StopPlayingMusic("home_replay");
		Engine::GetAudioManager().StopPlayingMusic("walk");

		if (!Engine::GetAudioManager().IsMusicFinished("home_intro"))
		{
			playing = false;
		}
		else
		{
			playing_replay = false;
		}
	}
}

void Mode2::FixedUpdate(double dt)
{
	if (GetGSComponent<GameObjectManager>() && !option->isOpened()) {
		GetGSComponent<GameObjectManager>()->FixedUpdateAll(dt);
	}
}

void Mode2::Draw() {
	if (GetGSComponent<Background>() && GetGSComponent<Cam>()) {
		GetGSComponent<Background>()->Draw(*GetGSComponent<Cam>());
	}
	GetGSComponent<GameObjectManager>()->DrawAll();
	dialog_ptr->Draw();

	today_fish_popup->SetPop(true);

	Engine::GetFontManager().PrintText(FontType::AlumniSans_Medium, FontAlignment::LEFT, "Trade your fish for " + std::to_string(inven_ptr->TodayFishPrice()) + " shiny coins!", { -262.f,142.f }, 0.032f, { 0.f,0.f,0.f }, 1.0f);

	if (inven_ptr->GetIsOpened())
	{
		Engine::GetFontManager().PrintText(FontType::AlumniSans_Medium, FontAlignment::LEFT, std::to_string(inven_ptr->GetMoney()), { 285.f,157.f }, 0.05f, { 0.f,0.f,0.f }, 1.0f);
		Engine::GetFontManager().PrintText(FontType::AlumniSans_Medium, FontAlignment::LEFT, "Show the maximum dash distance", { -230.f,68.f }, 0.032f, { 1.f,1.f,1.f }, 1.0f);
		Engine::GetFontManager().PrintText(FontType::AlumniSans_Medium, FontAlignment::LEFT, "Show the amount of fuel left", { -230.f,27.f }, 0.032f, { 1.f,1.f,1.f }, 1.0f);

		if (inven_ptr->GetFishState())
		{
			float currentY = 5.5f;
			int printed = 0;

			int totalCaptured = 0;
			for (int i = 0; i < 7; ++i)
				if (fishCollection[i] != 0)
					totalCaptured++;

			for (int i = 0; i < 7; ++i) {
				if (fishCollection[i] != 0) {
					Engine::GetFontManager().PrintText(
						FontType::AlumniSans_Medium, FontAlignment::LEFT,
						std::to_string(inven_ptr->HowManyFishes(i)),
						{ 190.f, currentY + 50 },
						0.05f,
						{ 1.f, 1.f, 1.f },
						1.0f
					);

					printed++;
					if (printed < totalCaptured - 1) currentY -= 36.f;
					else if (printed < totalCaptured) currentY -= 43.f;
				}
			}

			if (!sell_popup->GetPop() && Engine::GetInput().MouseButtonJustReleased(SDL_BUTTON_LEFT) && flag)
			{
				sell_popup->SetPop(true);
				Engine::GetIconManager().ShowIconByGroup("FishPopUp");
				Engine::GetIconManager().ShowIconById("fish" + std::to_string(n) + "_popping");
				flag = false;
				inven_ptr->which_fish_sellected = n - 1;
			}
			if (sell_popup->GetPop())
			{
				Engine::GetFontManager().PrintText(FontType::Bold, FontAlignment::LEFT, std::to_string(inven_ptr->HowMuchSold()), { -5.f,-40.f }, 0.05f, { 1.f,1.f,1.f }, 1.0f);
				Engine::GetFontManager().PrintText(FontType::Bold, FontAlignment::LEFT, (n - 1 != inven_ptr->GetTodayFishIndex()) ? std::to_string(fishGenerator->ReturnFishMoney(n)) : std::to_string(inven_ptr->TodayFishPrice()), { -8.f,-18.f }, 0.03f, { 0.f,0.f,0.f }, 1.0f);
			}
		}
		else
		{
			sell_popup->SetPop(false);
			Engine::GetIconManager().HideIconByGroup("FishPopUp");
			Engine::GetIconManager().HideIconByGroup("FishPopping");
		}
	}
	else
	{
		//Engine::GetFontManager().PrintText(FontType::AlumniSans_Medium, FontAlignment::LEFT, "Click Ship to Start the Game", { -100.f,-150.f }, 0.05f, { 0.f,0.f,0.f }, 0.5f);
		//Engine::GetFontManager().PrintText(FontType::AlumniSans_Medium, FontAlignment::LEFT, "Click Computer to Equip Module", { 30.f,30.f }, 0.05f, { 1.f,1.f,1.f }, 0.5f);
		sell_popup->SetPop(false);
		Engine::GetIconManager().HideIconByGroup("FishPopUp");
		Engine::GetIconManager().HideIconByGroup("FishPopping");
	}

	if (is_holding_sea_icon) {
		std::cout << hold_timer << std::endl;

		progress = std::clamp(hold_timer / 5.0f, 0.0f, 1.0f);
		Engine::GetRender().DrawCircleProgress({ Engine::GetInput().GetMousePos().mouseCamSpaceX, Engine::GetInput().GetMousePos().mouseCamSpaceY }, 30.0f,progress,1.f );
	}
}

void Mode2::Unload() {

	Engine::GetSaveDataManager().SetFishData(
		inven_ptr->GetMoney(),
		inven_ptr->fishCollection
	);


	ModuleData m1{
		inven_ptr->FirstModuleBought(),
		module_ptr->IsFirstSetted(),
		inven_ptr->GetX1Pos()
	};

	ModuleData m2{
		inven_ptr->SecondModuleBought(),
		module_ptr->IsSecondSetted(),
		inven_ptr->GetX2Pos()
	};


	Engine::GetSaveDataManager().SetModuleData(m1, m2);
	Engine::GetSaveDataManager().Save();

	Engine::GetAudioManager().StopAllChannels();
	GetGSComponent<GameObjectManager>()->Unload();
	GetGSComponent<Background>()->Unload();
	Engine::GetIconManager().Unload();

	ClearGSComponents();
	dialog_ptr->Unload();
	playing = false;

	background = nullptr;
	scenario = nullptr;
	hold_timer = 0.f;
}