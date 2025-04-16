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

#include <cmath>

#include "States.h"
#include "Background.h"
#include "Ship.h"
#include "Mode2.h"
#include "Mouse.h"
#include "DialogBox.h"
#include "WaterRippleEffect.h"
#include "ScenarioComponent.h"

#include <iostream>     // for debug
#include "Module.h"

Mode2::Mode2() {}

int dialog_test_int = 0;

void Mode2::Load() {

#ifdef _DEBUG
    AddGSComponent(new ShowCollision());
#else
#endif
    //shader
    Engine::GetShaderManager().LoadShader("water_ripple", "assets/shaders/default.vert", "assets/shaders/water_ripple.frag");    
    Engine::GetShaderManager().LoadShader("icon", "assets/shaders/default.vert", "assets/shaders/edge_detection.frag");

    // audio
    Engine::GetAudioManager().LoadMusic("assets/audios/bgm_original.wav", "Home_bgm", false);

    // compenent
    AddGSComponent(new GameObjectManager());

    // player
    player_ptr = new Player({ 0, -115 });
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
    dialog_ptr = new Dialog({0,0});
    GetGSComponent<GameObjectManager>()->Add(dialog_ptr);

    //ScenarioComponent
    AddGSComponent(new ScenarioComponent(Engine::GetEventManager(), Engine::GetSaveDataManager(), dialog_ptr));
    
    Engine::GetLogger().LoadSaveFile();

    // Module
   module_ptr = new Module({ 0, 0 });
    GetGSComponent<GameObjectManager>()->Add(module_ptr);

    // Inven
    inven_ptr = new Inven({350,0});
    GetGSComponent<GameObjectManager>()->Add(inven_ptr);

    shop_ptr = new Shop({ -350, 0 });
    GetGSComponent<GameObjectManager>()->Add(shop_ptr);

    // Mouse
    GetGSComponent<GameObjectManager>()->Add(new Mouse);

    std::cout << "Left money : " << Engine::GetGameStateManager().GetGSComponent<Fish>()->GetMoney() << std::endl;
}

void Mode2::Update(double dt) {
    //audio play
    if (!playing)
    {
        Engine::GetAudioManager().PlayMusics("Home_bgm");
        playing = true;
    }

    UpdateGSComponents(dt);
    GetGSComponent<GameObjectManager>()->UpdateAll(dt);
    GetGSComponent<Cam>()->Update(dt, {}, false);
    
    ////float moving~
    time += float(dt);
    //ship_ptr->SetVelocity({ 0, -(y_limit * frequency * std::cos(frequency * float(time))) });

#ifdef _DEBUG
    if (Engine::GetInput().KeyJustPressed(Input::Keys::Q)) {
        Engine::GetGameStateManager().ClearNextGameState();
        Engine::GetGameStateManager().SetNextGameState(static_cast<int>(States::Mode1));
    }
    if (Engine::GetInput().KeyJustPressed(Input::Keys::W)) {
        Engine::GetGameStateManager().ReloadState();
    }
#else
#endif

    Engine::GetIconManager().AddIcon("go_shop", { 276,4.5 }, 2.0f, false, false, true);
    Engine::GetIconManager().AddIcon("ship", { 0,-250 }, 1.0f, false, false, true);

    Icon* icon = Engine::GetIconManager().GetCollidingIconWithMouse({ Engine::GetInput().GetMousePos().mouseCamSpaceX ,Engine::GetInput().GetMousePos().mouseCamSpaceY });
    bool clicked = Engine::GetInput().MouseButtonJustPressed(SDL_BUTTON_LEFT);

    if (icon != nullptr) {
        if ((icon->GetAlias() == "ship") && clicked && !inven_ptr->GetIsOpened()) {
            Engine::GetGameStateManager().ClearNextGameState();
            Engine::GetGameStateManager().SetNextGameState(static_cast<int>(States::Mode1));
        }
    }

    // Open Inven
    if (inven_ptr->Open())
    {
        inven_ptr->SetIsOpened(true);
    }

    if(inven_ptr->GetIsOpened() && Engine::GetInput().KeyJustReleased(Input::Keys::Esc))
    {
        inven_ptr->SetIsOpened(false);
    }


    //Dialog
    //에셋에 있는  en 파일하고 같이 보면 이해가 더 쉬울거임
    if (Engine::GetInput().KeyJustPressed(Input::Keys::Z) && !isLoaded) {
        //묶음으로 되어있는거 출력 입력 받는 id는 알아서 설정해도 됨
        dialog_ptr->LoadDialogGroup("dialog-1", 0.05);
        isLoaded = true;
    }
    if (Engine::GetInput().KeyJustPressed(Input::Keys::Enter)) {
        //묶음으로 출력된 다이얼로그 다음 으로 넘겨주기
        dialog_ptr->NextLine(); 
    }
    if ((Engine::GetInput().KeyJustPressed(Input::Keys::X) && !isLoaded)) {

        //array 안에 string이 여러개 있으면 랜덤 출력함
        dialog_ptr->LoadRandomDialog("dialog-2", 0.05);
        isLoaded = true;
    }


    if (Engine::GetInput().KeyJustPressed(Input::Keys::C) && isLoaded) {
        //다이얼로그 숨김 뒤어 unload nullptr 처리해주기는 하는데 그냥 이건 안보이게만 하는거임
        dialog_ptr->Hide();
        isLoaded = false;
    }

    dialog_ptr->Update(dt);
}

void Mode2::FixedUpdate(double dt)
{}

void Mode2::Draw() {
    GetGSComponent<Background>()->Draw(*GetGSComponent<Cam>());
    GetGSComponent<GameObjectManager>()->DrawAll();
    dialog_ptr->Draw();

    if (inven_ptr->GetIsOpened())
    {
        Engine::GetFontManager().PrintText(FontType::Bold, std::to_string(inven_ptr->GetMoney()), { 285.f,157.f }, 0.05f, { 0.f,0.f,0.f }, 1.0f);

        if (inven_ptr->GetFishState())
        {
            Engine::GetFontManager().PrintText(FontType::Bold, std::to_string(inven_ptr->HowMuchSold()), { -5.f,75.f }, 0.05f, { 1.f,1.f,1.f }, 1.0f);

            float currentY = 5.5f;
            int printed = 0;

            // 총 몇 개 출력할 건지 먼저 계산
            int totalCaptured = 0;
            for (int i = 0; i < 3; ++i)
                if (fishCaptureCount[i] != 0)
                    totalCaptured++;

            for (int i = 0; i < 3; ++i) {
                if (fishCaptureCount[i] != 0) {
                    Engine::GetFontManager().PrintText(
                        FontType::Bold,
                        std::to_string(inven_ptr->HowManyFishes(i)),
                        { 190.f, currentY },
                        0.05f,
                        { 1.f, 1.f, 1.f },
                        1.0f
                    );

                    printed++;
                    if (printed < totalCaptured - 1) currentY -= 36.f; // 첫~중간 항목은 -36
                    else if (printed < totalCaptured) currentY -= 43.f; // 마지막 바로 전 항목은 -43
                }
            }
        }
    }
    else
    {
        Engine::GetFontManager().PrintText(FontType::Bold, "Click Ship to Start the Game", { -100.f,-150.f }, 0.05f, { 0.f,0.f,0.f }, 0.5f);
        Engine::GetFontManager().PrintText(FontType::Bold, "Click Computer to Equip Module", { 30.f,30.f }, 0.05f, { 1.f,1.f,1.f }, 0.5f);
    }
}

void Mode2::Unload() {
    Engine::GetLogger().WriteSaveFile(inven_ptr->fishCollection, inven_ptr->GetMoney(), inven_ptr->FirstModuleBought(), module_ptr->IsFirstSetted(), inven_ptr->GetX1Pos(), inven_ptr->SecondModuleBought(), module_ptr->IsSecondSetted(), inven_ptr->GetX2Pos());

    GetGSComponent<GameObjectManager>()->Unload();
    Engine::GetAudioManager().StopAllChannels();
    GetGSComponent<Background>()->Unload();
    ClearGSComponents();
    background = nullptr;
    dialog_ptr->Unload();
    playing = false;
}