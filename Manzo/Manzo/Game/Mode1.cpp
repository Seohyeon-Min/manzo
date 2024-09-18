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
#include "States.h"
#include "Mode1.h"
#include "../Engine/Timer.h"
#include "../Engine/ShowCollision.h"
#include "BeatSystem.h"
#include "Ship.h"
#include "Rock.h"
#include "../Engine/AudioManager.h"
#include <iostream>

Mode1::Mode1()
{}


void Mode1::Load() {

#ifdef _DEBUG
    AddGSComponent(new CS230::ShowCollision());
#else
#endif
    // compenent
    AddGSComponent(new CS230::GameObjectManager());
    AddGSComponent(new Background());
    AddGSComponent(new Beat());
    AddGSComponent(new AudioManager());

    // camera
    AddGSComponent(new CS230::Camera({ { 0.15 * Engine::GetWindow().GetSize().x, 0 }, { 0.35 * Engine::GetWindow().GetSize().x, 0 } }));
    GetGSComponent<CS230::Camera>()->SetPosition({ 0, 0 });

    // ship
    GetGSComponent<CS230::GameObjectManager>()->Add(new Ship({500,300}));

    // rock
    GetGSComponent<CS230::GameObjectManager>()->Add(new Rock({ 200,200 }));

    // audio
    Mix_Music* sample = GetGSComponent<AudioManager>()->LoadMusic("Assets/Audio/basic_beat_100_4.wav", "sample");
    if (sample) {
        GetGSComponent<AudioManager>()->PlayMusic(sample, -1);
    }


}

void Mode1::Update(double dt) {
    UpdateGSComponents(dt);
    GetGSComponent<CS230::GameObjectManager>()->UpdateAll(dt);
    //GetGSComponent<CS230::ParticleManager<Particles::Smoke>>()->Update(dt);
}

void Mode1::Draw() {
    Engine::GetWindow().Clear(0x000000FF);

    GetGSComponent<CS230::GameObjectManager>()->DrawAll(GetGSComponent<CS230::Camera>()->GetMatrix());
    //GetGSComponent<CS230::ParticleManager<Particles::Smoke>>()->Draw(GetGSComponent<CS230::Camera>()->GetMatrix());
}

void Mode1::Unload() {

    GetGSComponent<CS230::GameObjectManager>()->Unload();
    ClearGSComponents();
}
