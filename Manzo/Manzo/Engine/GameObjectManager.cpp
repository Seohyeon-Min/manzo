/*
Copyright (C) 2023 DigiPen Institute of Technology
Reproduction or distribution of this file or its contents without
prior written consent is prohibited
File Name:  GameObjectManager.cpp
Project:    CS230 Engine
Author:     Seohyeon Min
Created:    Aprill 29, 2023
*/


#include "GameObjectManager.h"
#include "MapManager.h"
#include "Camera.h"

void GameObjectManager::Add(GameObject* object)
{
	objects.push_back(object);
}

void GameObjectManager::Unload()
{
	objects.clear();
}

void GameObjectManager::UpdateAll(double dt)
{
	std::vector<GameObject*> destroy_objects;
	for (auto object : objects) {
		object->Update(dt);
		if (object->Destroyed()) {
			destroy_objects.push_back(object);
		}
	}
	for (auto d_object : destroy_objects) {
		objects.remove(d_object);
		delete d_object;
	}
}

void GameObjectManager::FixedUpdateAll(double dt)
{
	for (auto object : objects) {
		object->FixedUpdate(dt);
	}
}

void GameObjectManager::DrawAll()
{
	auto camera_bounds = Engine::GetGameStateManager().GetGSComponent<Cam>()->GetBounds();

	for (auto object : objects) {
		if (object->isCameraFixed()) {
			object->Draw();
		}
		else if (object->IsVisible(camera_bounds)) {
			object->Draw();
		}

	}

	Engine::GetRender().RenderAll();
}


void GameObjectManager::CollisionTest() {
	for (auto object_1 : objects) {
		for (auto object_2 : objects) {
			if (object_1 != object_2 && object_1->CanCollideWith(object_2->Type())) {
				if (object_1->IsCollidingWith(object_2)) {
					Engine::GetLogger().LogEvent("Collision Detected: " + object_1->TypeName() + " and " + object_2->TypeName());
					object_1->ResolveCollision(object_2);
				}
			}
		}
	}
}


vec2 GameObjectManager::FindNearestRock(GameObject* object) {
	GameObjectManager* gameObjectManager = Engine::GetGameStateManager().GetGSComponent<GameObjectManager>();
	if (gameObjectManager == nullptr) {
		Engine::GetLogger().LogError("GameObjectManager not found");
		return { 0,0 };
	}

	vec2 nearestRockpoints;
	float nearestDistance = std::numeric_limits<float>::max();
	vec2 object_Position = object->GetPosition();

	for (GameObject* gameObj : gameObjectManager->objects) {
		if (gameObj->Type() == GameObjectTypes::Rock) {
			Rock* rock = static_cast<Rock*>(gameObj);
			std::vector<vec2> rockPoints = rock->GetRockGroup()->GetPoints();

			for (const vec2& rockPoint : rockPoints) {
				float distance = sqrtf((float)pow(rockPoint.x - object_Position.x, 2) + (float)pow(rockPoint.y - object_Position.y, 2));

				if (distance < nearestDistance) {
					nearestDistance = distance;
					nearestRockpoints = rockPoint;
				}
			}
		}
	}

	return nearestRockpoints;
}

void GameObjectManager::Remove(GameObject* object) {
	auto it = std::find(objects.begin(), objects.end(), object);

	if (it != objects.end()) {
		objects.erase(it);
		std::cout << "GameObject Removed from Vector.\n";
	}
	else {
		std::cout << "GameObject Not Found.\n";
	}
}