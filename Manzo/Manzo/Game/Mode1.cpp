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

#include "States.h"
#include "Mode1.h"
#include "BeatSystem.h"
#include "Background.h"
#include "Ship.h"
#include "Fish.h"
#include "Reef.h"

#include <iostream>

Mode1::Mode1()
{
}


void Mode1::Load() {

#ifdef _DEBUG
    AddGSComponent(new CS230::ShowCollision());
#else
#endif
    // compenent
    AddGSComponent(new CS230::GameObjectManager());
    AddGSComponent(new Beat());
    AddGSComponent(new AudioManager());


    //// camera
    camera = new CS230::Camera({ {1280 / 2 , 720 / 2 }, {1280 / 2, 720 / 2 } });
    AddGSComponent(camera);

    background = new Background({ 0, 0 }, *camera);

    GetGSComponent<CS230::GameObjectManager>()->Add(background);
    //// ship
    ship_ptr = new Ship({ 0, 0 });
    GetGSComponent<CS230::GameObjectManager>()->Add(ship_ptr);


    vec2 playerPosition = ship_ptr->GetPosition();
    GetGSComponent<CS230::Camera>()->SetPosition({ playerPosition.x - 1280 / 2, playerPosition.y - 720 / 2 });
    GetGSComponent<CS230::Camera>()->SetLimit({ { 0,0 }, { (Engine::window_width), (Engine::window_height) } });
    //// audio
    Mix_Music* sample = GetGSComponent<AudioManager>()->LoadMusic("assets/audios/basic_beat_100_5.wav", "sample");
    if (sample) {
        GetGSComponent<AudioManager>()->PlayMusic(sample, -1);
    }

    //// to generate fish
    fishGenerator = new FishGenerator();

    //// reef
    reef = new Reef({ -400,200 });
    GetGSComponent<CS230::GameObjectManager>()->Add(reef);
}

void Mode1::Update(double dt) {
    GetGSComponent<CS230::Camera>()->Update(ship_ptr->GetPosition());
    UpdateGSComponents(dt);
    GetGSComponent<CS230::GameObjectManager>()->UpdateAll(dt);

    std::cout << "x: " << camera->GetPosition().x << ", y : " << camera->GetPosition().y << std::endl;
	fishGenerator->GenerateFish(dt);

    //get money
    if (fish)
    {
        if (ship_ptr->IsCollidingWith(fish) && !fish->collided)
        {
            fish->collided = true;
            money++;
        }
    }
}

void Mode1::Draw() {
    mat3 camera_matrix = GetGSComponent<CS230::Camera>()->GetMatrix();

    GetGSComponent<CS230::GameObjectManager>()->DrawAll(camera_matrix);
}

void Mode1::Unload() {
	GetGSComponent<CS230::GameObjectManager>()->Unload();
	fishGenerator->DeleteFish();
	ClearGSComponents();
}
