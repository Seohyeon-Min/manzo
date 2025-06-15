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
#include "MathUtils.h"

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
		if (d_object != nullptr) {
			objects.remove(d_object);
			delete d_object;
		}
	}
}

void GameObjectManager::FixedUpdateAll(double dt)
{
	if (objects.empty()) return;

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
		else if (true) {
			object->Draw();
		}

	}

	Engine::GetRender().RenderAll();
}


void GameObjectManager::CollisionTest() 
{
	if (!collision_on) {
		return;
	}
	for (auto object_1 : objects) {
		//if (!object_1->isActive(camera_bounds)) continue; <--- it is needfull
		for (auto object_2 : objects) {
			if (object_1 != object_2 && object_1->CanCollideWith(object_2->Type())) {
				if (object_1->IsCollidingWith(object_2)) {
					//Engine::GetLogger().LogEvent("Collision Detected: " + object_1->TypeName() + " and " + object_2->TypeName());
					object_1->ResolveCollision(object_2);
				}
			}
		}
	}
}


vec2 GameObjectManager::FindNearestRockPoint(GameObject* object) {

	vec2 nearestRockpoints;
	float nearestDistance = std::numeric_limits<float>::max();
	vec2 object_Position = object->GetPosition();

	for (GameObject* gameObj : objects) {
		Rock* rock = nullptr;
		std::vector<vec2> rockPoints;

		if (gameObj->Type() == GameObjectTypes::Rock) {
			rock = static_cast<Rock*>(gameObj);
			rockPoints = rock->GetRockGroup()->GetPoints();
		}
		else if (gameObj->Type() == GameObjectTypes::ObstacleRock) {
			rock = static_cast<ObstacleRock*>(gameObj);
			rockPoints = rock->GetRockGroup()->GetPoints();
		}
		else {
			continue;
		}
			for (const vec2& rockPoint : rockPoints) {
				float distance = (rockPoint - object_Position).LengthSquared();

				if (distance < nearestDistance) {
					nearestDistance = distance;
					nearestRockpoints = rockPoint;
				}
			}
	}

	return nearestRockpoints;
}


Rock* GameObjectManager::FindNearestRock(GameObject* object) {
	Rock* nearestRock = nullptr;
	float nearestDistance = std::numeric_limits<float>::max();
	vec2 object_Position = object->GetPosition();

	auto camera_bounds = Engine::GetGameStateManager().GetGSComponent<Cam>()->GetBounds();

	for (GameObject* gameObj : objects) {
		Rock* rock = nullptr;
		std::vector<vec2> rockPoints;

		if (gameObj->Type() == GameObjectTypes::ObstacleRock/* && gameObj->IsVisible(camera_bounds)*/) {
			rock = static_cast<ObstacleRock*>(gameObj);
			rockPoints = rock->GetPoints();
		}
		else if (gameObj->Type() == GameObjectTypes::Rock/* && gameObj->IsVisible(camera_bounds)*/) {
			rock = static_cast<Rock*>(gameObj);
			rockPoints = rock->GetPoints();
		}
		if (!rock) continue;
		

		for (size_t i = 0; i < rockPoints.size(); ++i) {
			vec2 p1 = rockPoints[i];
			vec2 p2 = rockPoints[(i + 1) % rockPoints.size()];

			vec2 closestPoint = ClosestPoint(object_Position, p1, p2);
			float distance = (closestPoint - object_Position).LengthSquared();

			if (distance < nearestDistance) {
				nearestDistance = distance;
				nearestRock = rock;
			}
		}
	}

	return nearestRock;
}


void GameObjectManager::Remove(GameObject* object) {
	auto it = std::find(objects.begin(), objects.end(), object);

	if (it != objects.end()) {
		objects.erase(it);
	}
	else {
	}
}
