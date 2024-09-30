#include "../Engine/Engine.h"
#include "../Engine/ShowCollision.h"
#include "../Engine/Collision.h"

#include "States.h"
#include "Sea.h"
#include "Fish.h"
#include "Reef.h"

#include <iostream>

Sea::Sea() {

	timer = new CS230::Timer(fish_timer);
}

void Sea::Load() {
	counter = 0;
	AddGSComponent(new CS230::GameObjectManager());

#ifdef _DEBUG
	AddGSComponent(new CS230::ShowCollision());
#endif
	fish = new Fish();
	reef = new Reef({ 100,550 });
	GetGSComponent<CS230::GameObjectManager>()->Add(fish);
	GetGSComponent<CS230::GameObjectManager>()->Add(reef);
	//GetGSComponent<CS230::GameObjectManager>()->Add(new Reef({ 100,550 }));
	//GetGSComponent<CS230::GameObjectManager>()->Add(new Reef({ 240,220 }));
	//GetGSComponent<CS230::GameObjectManager>()->Add(new Reef({ 600,370 }));

	fishList.clear();
}

void Sea::Draw() {
	Engine::GetWindow().Clear(0xFFFFFFFF);
	DrawRectangleGradientV(0, 0, Engine::GetWindow().GetSize().x, Engine::GetWindow().GetSize().y, { 24,129,140,255 }, { 2,94,115,255 });

	GetGSComponent<CS230::GameObjectManager>()->DrawAll(Math::TransformationMatrix());
}

void Sea::Update(double dt) {
	UpdateGSComponents(dt);
	GetGSComponent<CS230::GameObjectManager>()->UpdateAll(dt);
	timer->Update(dt);

	if (timer->timer == 0)
	{
		Fish* newFish = new Fish();
		fishList.push_back(newFish);
		GetGSComponent<CS230::GameObjectManager>()->Add(newFish);
		timer->Reset();
	}

    for (Fish* fish : fishList)
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
    }
}

void Sea::Unload() {
	GetGSComponent<CS230::GameObjectManager>()->Unload();

	for (Fish* fish : fishList)
	{
		delete fish;
	}

	fishList.clear();

	ClearGSComponents();
}
