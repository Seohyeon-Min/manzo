#include "Fish.h"
#include "Effect.h"
#include "ScreenWrap.h"

#include "../Engine/GameObjectManager.h"
#include "../Engine/Camera.h"
#include "../Engine/Engine.h"

#include <random>
#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include <cmath>
#include <algorithm>
#include <map>
#include "FishEcosystem.h"
#include "../Engine/MathUtils.h"

std::map<int, int> fishCollection;

Fish::Fish(int index, Fish* parent) : GameObject({ 0, 0 }) {
	map = Engine::GetGameStateManager().GetGSComponent<MapManager>()->GetMap(0);
	ship = Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->GetGOComponent<Ship>();

	if (parent == nullptr) {
		bool isLeft = true; // rand() % 2;

		start_position = map->Spawn();//map->MaskToWorld(4700,5900);//{ startX , startY };
		SetPosition(start_position);

		//SetVelocity({ 0,0 });

		SetVelocity(isLeft ? Engine::GetGameStateManager().GetGSComponent<FishGenerator>()->fishBook[index].velocity : -Engine::GetGameStateManager().GetGSComponent<FishGenerator>()->fishBook[index].velocity);
		SetFlipX(!isLeft);

		SetScale(Engine::GetGameStateManager().GetGSComponent<FishGenerator>()->fishBook[index].scale);
		type = Engine::GetGameStateManager().GetGSComponent<FishGenerator>()->fishBook[index].type;
	}
	else {
		index = 2;
		type = parent->type;
		parentFish = parent;
	}

	AddGOComponent(new Sprite(Engine::GetGameStateManager().GetGSComponent<FishGenerator>()->fishBook[index].filePath, this));
	Engine::GetGameStateManager().GetGSComponent<FishGenerator>()->fishCnt++;
}

bool Fish::CanCollideWith(GameObjectTypes other_object) {
	switch (other_object) {
	case GameObjectTypes::Ship:
		return true;
	default:
		return false;
	}
}

void Fish::ResolveCollision(GameObject* other_object) {
	switch (other_object->Type()) {
	case GameObjectTypes::Ship:
		Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->Add(new CaptureEffect(GetPosition()));
		if (!collided)
		{
			collided = true;
			Engine::GetGameStateManager().GetGSComponent<FishGenerator>()->SetFishCnt(Engine::GetGameStateManager().GetGSComponent<FishGenerator>()->GetFishCnt() - 1);
			Engine::GetGameStateManager().GetGSComponent<FishGenerator>()->SetMoney(Engine::GetGameStateManager().GetGSComponent<FishGenerator>()->money + Engine::GetGameStateManager().GetGSComponent<FishGenerator>()->moneys[type - 1]);
			fishCollection[type - 1]++;
		}
		auto& list = Engine::GetGameStateManager().GetGSComponent<FishGenerator>()->fishList;
		list.erase(std::remove(list.begin(), list.end(), this), list.end());
		this->Destroy();
		break;
	}
}

void Fish::Update(double dt) {
	GameObject::Update(dt);

	SetFlipX(GetVelocity().x <= 0);

	if (parentFish != nullptr) {
		if (parentFish->Destroyed()) {
			parentFish = nullptr;
		}
		else {
			SetVelocity(parentFish->GetVelocity());
			SetRotation(parentFish->GetRotation());
			return;
		}
	}

	if (GetPosition().x < 0.f || GetPosition().x > 5000.f)
	{
		auto& list = Engine::GetGameStateManager().GetGSComponent<FishGenerator>()->fishList;
		list.erase(std::remove(list.begin(), list.end(), this), list.end());
		this->Destroy();
	}


	if (ship->CatchFish(GetPosition()))
	{
		Pattern();
	}
	else
	{
		AvoidRock();
		flag = true;
	}
}

void Fish::Draw() {
	GameObject::Draw(DrawLayer::DrawLast);
}

void Fish::Pattern()
{
	vec2 shipPos = Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->GetGOComponent<Ship>()->GetPosition();
	vec2 myPos = this->GetPosition();
	vec2 toShip = shipPos - myPos;
	float distance = toShip.Length();

	float escapeRange, maxEscapeSpeed, escapeStrength;
	vec2 awayFromShip, newVelocity, circleDir;


	switch ((type - 1) % 3)
	{
	case 0:
		escapeRange = 300.0f;
		maxEscapeSpeed = 150.0f;

		escapeStrength = std::clamp(1.0f - (distance / escapeRange), 0.0f, 1.0f);

		awayFromShip = -normalize(toShip);

		newVelocity = Lerp(Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->GetGOComponent<Ship>()->GetVelocity(), awayFromShip * maxEscapeSpeed, escapeStrength);

		this->SetVelocity(newVelocity);
		break;

	case 1:
		escapeRange = 500.0f;
		maxEscapeSpeed = 350.0f;

		escapeStrength = std::clamp(1.0f - (distance / escapeRange), 0.0f, 1.0f);

		awayFromShip = -normalize(toShip);

		newVelocity = Lerp(Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->GetGOComponent<Ship>()->GetVelocity(), awayFromShip * maxEscapeSpeed, escapeStrength);

		this->SetVelocity(newVelocity);
		break;

	case 2:
		escapeRange = 400.0f;
		maxEscapeSpeed = 200.0f;

		if (toShip.Length() < 1e-5f) {
			toShip = vec2(1.0f, 0.0f);
		}

		distance = toShip.Length();
		escapeStrength = std::max(0.2f, std::clamp(1.0f - (distance / escapeRange), 0.0f, 1.0f));

		circleDir = normalize(vec2(-toShip.x, -toShip.y));

		vec2 circleVelocity = circleDir * maxEscapeSpeed * escapeStrength;

		this->SetVelocity(circleVelocity);
		break;
	}
}

void Fish::AvoidRock()
{
	if (!map->IsMaskTrue(GetPosition()))
	{
		int max_attempts = 100;
		float try_distance = 100.0f;

		for (int i = 0; i < max_attempts; ++i) {
			float angle_deg = RandomFloat(0.0f, 360.0f);
			float angle_rad = glm::radians(angle_deg);
			vec2 dir = { cos(angle_rad), sin(angle_rad) };
			vec2 testPos = GetPosition() + dir * try_distance;

			if (map->IsMaskTrue(testPos)) {
				SetVelocity(dir * GetVelocity().Length());
				break;
			}
		}
	}
}
