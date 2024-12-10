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
#include "../Engine/UIManager.h"
#include <cmath>

#include "States.h"
#include "Background.h"
#include "BeatSystem.h"
#include "Ship.h"
#include "Mode2.h"
#include "Mouse.h"
#include "DialogBox.h"


#include <iostream>     // for debug

Mode2::Mode2() {}

int dialog_test_int = 0;//������ ����

void Mode2::Load() {

#ifdef _DEBUG
    AddGSComponent(new ShowCollision());
#else
#endif

    
    
    // compenent
    AddGSComponent(new GameObjectManager());

    //// ship
    ship_ptr = new Ship({ 0, -250 });
    GetGSComponent<GameObjectManager>()->Add(ship_ptr);

    //// camera
    AddGSComponent(new Cam());
    vec2 playerPosition = ship_ptr->GetPosition();
    GetGSComponent<Cam>()->SetPosition({ playerPosition.x, 0 });


    //// background
    background = new Background();
    AddGSComponent(background);
    background->Add("assets/images/background/house.png", 0.25f);

    /// Dialog
    dialog = new Dialog();
    AddGSComponent(dialog);
    dialog->Unload();


    AddGSComponent(new UIManager());
    ui_manager = GetGSComponent<UIManager>();
    ui_manager->AddUI(std::make_unique<Mouse>());

    //// audio
    //Mix_Music* sample = GetGSComponent<AudioManager>()->LoadMusic("assets/audios/basic_beat_100_4.wav", "sample");
    //if (sample) {
    //    GetGSComponent<AudioManager>()->PlayMusic(sample, -1);
    //}

    // skill
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

    // UI
    AddGSComponent(new UIManager());
    ui_manager = GetGSComponent<UIManager>();
    ui_manager->AddUI(std::make_unique<Mouse>());
    AddGSComponent(new Shop());

    std::cout << "Left money : " << Engine::GetGameStateManager().GetGSComponent<Fish>()->GetMoney() << std::endl;
}

void Mode2::Update(double dt) {
    UpdateGSComponents(dt);
    GetGSComponent<GameObjectManager>()->UpdateAll(dt);
    GetGSComponent<Cam>()->Update(dt, ship_ptr->GetPosition(), false);
    skill_ptr->Update();
    

    //float moving~
    time += float(dt);
    ship_ptr->SetVelocity({ 0, -(y_limit * frequency * std::cos(frequency * float(time))) });

    if (Engine::GetInput().KeyJustPressed(Input::Keys::Q)) {
        Engine::GetGameStateManager().ClearNextGameState();
        Engine::GetGameStateManager().SetNextGameState(static_cast<int>(States::Mode1));
    }
    if (Engine::GetInput().KeyJustPressed(Input::Keys::W)) {
        Engine::GetGameStateManager().ReloadState();
    }
    if (Engine::GetInput().KeyJustPressed(Input::Keys::Space)) {
       dialog->LoadDialog(1, 0.1);
    }
   

}

void Mode2::FixedUpdate(double dt)
{

}

void Mode2::Draw() {
    GetGSComponent<Background>()->Draw(*GetGSComponent<Cam>());
    GetGSComponent<GameObjectManager>()->DrawAll();
    ui_manager->AddDrawCalls();
    dialog->Draw();
    
}

void Mode2::Unload() {
    GetGSComponent<GameObjectManager>()->Unload();
    GetGSComponent<Background>()->Unload();
    ui_manager->UnloadUI();
    ClearGSComponents();
    ship_ptr = nullptr;
    skill_ptr = nullptr;
    background = nullptr;
    dialog->Unload();
}