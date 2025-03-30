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

#include <cmath>

#include "States.h"
#include "Background.h"
#include "BeatSystem.h"
#include "Ship.h"
#include "Mode2.h"
#include "Mouse.h"
#include "DialogBox.h"


#include <iostream>     // for debug
#include "Module.h"

Mode2::Mode2() {}

int dialog_test_int = 0;

void Mode2::Load() {

#ifdef _DEBUG
    AddGSComponent(new ShowCollision());
#else
#endif
    // compenent
    AddGSComponent(new GameObjectManager());

    // ship
    ship_ptr = new Ship({ 0, -250 });
    GetGSComponent<GameObjectManager>()->Add(ship_ptr);

    // player
    player_ptr = new Player({ 0, -115 });
    GetGSComponent<GameObjectManager>()->Add(player_ptr);

    // camera
    AddGSComponent(new Cam());
    GetGSComponent<Cam>()->SetPosition({ 0, 0 });

    // background
    background = new Background();
    AddGSComponent(background);
    background->Add("assets/images/background/house.png", 0.25f);

    // Icon
    Engine::GetIconManager().LoadIconList();


    shop_ptr = new Shop();
    GetGSComponent<GameObjectManager>()->Add(shop_ptr);

    // Dialog
    dialog_ptr = new Dialog({0,0});
    GetGSComponent<GameObjectManager>()->Add(dialog_ptr);

    Engine::GetLogger().LoadSaveFile();

    // Module
   module_ptr = new Module({ 0, 0 });
    GetGSComponent<GameObjectManager>()->Add(module_ptr);

    // Inven
    inven_ptr = new Inven({0,0});
    GetGSComponent<GameObjectManager>()->Add(inven_ptr);
 
    Engine::GetShaderManager().LoadShader("icon", "assets/shaders/default.vert", "assets/shaders/edge_detection.frag");

    // Mouse
    GetGSComponent<GameObjectManager>()->Add(new Mouse);

    std::cout << "Left money : " << Engine::GetGameStateManager().GetGSComponent<Fish>()->GetMoney() << std::endl;
}

void Mode2::Update(double dt) {
    UpdateGSComponents(dt);
    GetGSComponent<GameObjectManager>()->UpdateAll(dt);
    GetGSComponent<Cam>()->Update(dt, {}, false);
    
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
    if (Engine::GetInput().KeyJustPressed(Input::Keys::Space) && !isLoaded) {
       dialog_ptr->LoadDialog(1, 0.05);
       isLoaded = true;
    }

    Engine::GetIconManager().AddIcon("go_shop", { 100,0 }, 1.0f, false, false, true);

    // Open Inven
    if (inven_ptr->Open())
    {
        if (!inven_ptr->GetIsOpened()) inven_ptr->SetIsOpened(true);
        else inven_ptr->SetIsOpened(false);
    }
}

void Mode2::FixedUpdate(double dt)
{}

void Mode2::Draw() {
    GetGSComponent<Background>()->Draw(*GetGSComponent<Cam>());
    GetGSComponent<GameObjectManager>()->DrawAll();
    dialog_ptr->Draw();
}

void Mode2::Unload() {
    Engine::GetLogger().WriteSaveFile(inven_ptr->fishCollection, inven_ptr->GetMoney(), module_ptr->IsFirstSetted(), inven_ptr->GetX1Pos(), module_ptr->IsSecondSetted(), inven_ptr->GetX2Pos());

    GetGSComponent<GameObjectManager>()->Unload();
    GetGSComponent<Background>()->Unload();
    ClearGSComponents();
    ship_ptr = nullptr;
    background = nullptr;
    dialog_ptr->Unload();
}