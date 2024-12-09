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

void GameObjectManager::Add(GameObject* object)
{
	objects.push_back(object);
}

void GameObjectManager::Unload()
{
	for (auto object : objects) {
		delete object;
	}
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
	for (auto object : objects) {
		object->Draw();
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