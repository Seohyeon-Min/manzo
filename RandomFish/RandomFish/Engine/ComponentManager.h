/*
Copyright (C) 2024 DigiPen Institute of Technology
Reproduction or distribution of this file or its contents without
prior written consent is prohibited
File Name:  ComponenetManager.h
Project:    CS230 Engine
Author:     Jonathan Holmes, Gyuwon Na
Created:    March 8, 2023
Updated:    May 9, 2024
*/


#pragma once
#include <vector>
#include "Component.h"

namespace CS230 {
    class ComponentManager {
    public:
        ~ComponentManager() { Clear(); }
        void UpdateAll(double dt) {
            for (Component* component : components) {
                component->Update(dt);
            }
        }
        void AddComponent(Component* component) {
            components.push_back(component);
        }
        template<typename T>
        T* GetComponent() {
            for (Component* component : components) {
                T* ptr = dynamic_cast<T*>(component);
                if (ptr != nullptr) {
                    return ptr;
                }
            }
            return nullptr;
        }
        template<typename T>
        void RemoveComponent() {
            auto it = std::find_if(
                components.begin(), components.end(), [](Component* element) {
                    return (dynamic_cast<T*>(element) != nullptr);
                }
            );
            delete* it;
            components.erase(it);
        }
        void Clear() 
        {
            components.clear();
        }
    private:
        std::vector<Component*> components;
    };
}
