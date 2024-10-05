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
#include "FishEcosystem.h"
#include "Reef.h"
#include "Fish.h"

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
	AddGSComponent(new Background());
	AddGSComponent(new Beat());
	AddGSComponent(new AudioManager());
	GetGSComponent<Background>()->Add("Assets/temp_back.png", 0.25);

	// ship
	ship_ptr = new Ship({ 1280 / 2, 720 / 2 });
	GetGSComponent<CS230::GameObjectManager>()->Add(ship_ptr);

	// camera
	AddGSComponent(new CS230::Camera({ {1280 / 2 , 720 / 2 }, {1280 / 2, 720 / 2 } }));
	Math::vec2 playerPosition = ship_ptr->GetPosition();
	GetGSComponent<CS230::Camera>()->SetPosition({ playerPosition.x - 1280 / 2, playerPosition.y - 720 / 2 });

	// audio
	Mix_Music* sample = GetGSComponent<AudioManager>()->LoadMusic("Assets/Audio/basic_beat_100_4.wav", "sample");
	if (sample) {
		GetGSComponent<AudioManager>()->PlayMusic(sample, -1);
	}

	fishGenerator = new FishGenerator();

	reef = new Reef({ 100,550 });
	GetGSComponent<CS230::GameObjectManager>()->Add(reef);
}

void Mode1::Update(double dt) {
	UpdateGSComponents(dt);
	GetGSComponent<CS230::GameObjectManager>()->UpdateAll(dt);
	GetGSComponent<CS230::Camera>()->Update(ship_ptr->GetPosition());

	fishGenerator->GenerateFish(dt);

	for (Fish* fish : fishGenerator->fishList)
	{
		Math::vec2 fishPos = fish->GetPosition();
		Math::vec2 reefPos = reef->GetPosition();

		Math::vec2 distanceVec = fishPos - reefPos;
		double distance = sqrt(pow(distanceVec.x, 2) + pow(distanceVec.y, 2));

		const double repellingForce = 50.0;
		const double pushAmountFactor = 0.5;
		const double minDistance = 100.0;
		static double timeCounter = 0.0;

		if (distance < minDistance) {
			Math::vec2 awayFromReef = distanceVec.Normalize();

			double pushAmount = (minDistance - distance) * pushAmountFactor;
			fish->UpdatePosition(awayFromReef * pushAmount);

			Math::vec2 steeringVec = awayFromReef;

			double strength = (minDistance - distance) / minDistance;
			steeringVec *= (repellingForce * strength * dt);

			fish->UpdateVelocity(steeringVec);
		}
		if (distance > minDistance)
		{
			timeCounter += dt;
			double verticalMovement = (timeCounter * 10.0) * 5.0;
			fishPos.y += verticalMovement;
		}

		fishPos.x += fish->GetVelocity().x * dt;
		fishPos.y += fish->GetVelocity().y * dt;

		//get money
		if (ship_ptr->IsCollidingWith(fish) && !fish->collided)
		{
			fish->collided = true;
			money++;
		}
	}
}

void Mode1::Draw() {
	Engine::GetWindow().Clear(0x000000FF);
	GetGSComponent<Background>()->Draw(*GetGSComponent<CS230::Camera>());
	GetGSComponent<CS230::GameObjectManager>()->DrawAll(GetGSComponent<CS230::Camera>()->GetMatrix());
}

void Mode1::Unload() {

	GetGSComponent<CS230::GameObjectManager>()->Unload();
	fishGenerator->DeleteFish();
	ClearGSComponents();
}
