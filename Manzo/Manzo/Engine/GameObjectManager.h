/*
Copyright (C) 2023 DigiPen Institute of Technology
Reproduction or distribution of this file or its contents without
prior written consent is prohibited
File Name:  GameObjectManager.h
Project:    CS230 Engine
Author:     Jonathan Holmes
Created:    March 8, 2023
*/

#pragma once
#include <list>
#include "GameObject.h"
#include "mat3.h"
#include "..\Game\Rock.h"

namespace Math { class TransformationMatrix; }


class GameObjectManager : public Component {
public:
    void Add(GameObject* object);
    void Unload();
    void UpdateAll(double dt);
    void FixedUpdateAll(double dt);
    void DrawAll();


    void CollisionTest();
    vec2 FindNearestRockPoint(GameObject* object);
    Rock* FindNearestRock(GameObject* object);
    void Remove(GameObject* object);
    void SetCollisionON(bool on) { collision_on = on; }

    template<typename T>
    T* GetGOComponent() {
        for (GameObject* object : objects) {
            T* ptr = dynamic_cast<T*>(object);
            if (ptr != nullptr) {
                return ptr;
            }
        }
        return nullptr;
    }

	std::list<GameObject*> GetAllObjects() const {
		return objects;
	}

private:
    std::list<GameObject*> objects;
    bool collision_on = true;
};
