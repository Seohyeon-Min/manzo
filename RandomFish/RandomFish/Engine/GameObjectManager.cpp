/*
Copyright (C) 2024 DigiPen Institute of Technology
Reproduction or distribution of this file or its contents without
prior written consent is prohibited
File Name:  GameObjectManager.cpp
Project:    CS230 Engine
Author:     Jonathan Holmes, Gyuwon Na
Created:    March 8, 2023
Updated:    May 23, 2024
*/

#include "GameObjectManager.h"

void CS230::GameObjectManager::Add(GameObject* object)
{
    objects.push_back(object);
}

void CS230::GameObjectManager::Unload()
{
    for (GameObject* object : objects)
    {
        delete object;
    }
    objects.clear();
}

void CS230::GameObjectManager::UpdateAll(double dt)
{
    std::vector<GameObject*> destroy_objects;

    for (auto object : objects)
    {
        object->Update(dt);
        if (object->Destroyed())
        {
            destroy_objects.push_back(object);
        }
    }

    for (auto object : destroy_objects) {
        auto it = std::find(objects.begin(), objects.end(), object);
        if (it != objects.end()) {
            objects.erase(it);
            delete object;
        }
    }
}

void CS230::GameObjectManager::DrawAll(Math::TransformationMatrix camera_matrix)
{
    for (auto object : objects)
    {
        object->Draw(camera_matrix);
    }
}

void CS230::GameObjectManager::CollisionTest() {
    for (GameObject* object_1 : objects) {
        for (GameObject* object_2 : objects) {
            if (object_1 != object_2 && object_1->CanCollideWith(object_2->Type())) {
                if (object_1->IsCollidingWith(object_2)) {
                    Engine::GetLogger().LogEvent("Collision Detected: " + object_1->TypeName() + " and " + object_2->TypeName());
                    object_1->ResolveCollision(object_2);
                }
            }
        }
    }
}
