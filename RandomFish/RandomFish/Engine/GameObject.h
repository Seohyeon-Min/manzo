/*
Copyright (C) 2024 DigiPen Institute of Technology
Reproduction or distribution of this file or its contents without
prior written consent is prohibited
File Name:  GameObject.h
Project:    CS230 Engine
Author:     Jonathan Holmes, Gyuwon Na
Created:    March 8, 2023
Updated:    May 23, 2024
*/

#pragma once
#include "Sprite.h"
#include "ComponentManager.h"

namespace Math { class TransformationMatrix; }
enum class GameObjectTypes;

namespace CS230 {
    class GameObject {
    public:
        friend class Sprite;

        GameObject(Math::vec2 position);
        GameObject(Math::vec2 position, double rotation, Math::vec2 scale);
        virtual ~GameObject() {}

        virtual GameObjectTypes Type() = 0;
        virtual std::string TypeName() = 0;
        bool IsCollidingWith(GameObject* other_object);
        virtual bool CanCollideWith(GameObjectTypes other_object_type);
        bool IsCollidingWith(Math::vec2 point);
        virtual void ResolveCollision(GameObject* other_object) { };

        virtual void Update(double dt);
        virtual void Draw(Math::TransformationMatrix camera_matrix);

        const Math::TransformationMatrix& GetMatrix();
        const Math::vec2& GetPosition() const;
        const Math::vec2& GetVelocity() const;
        const Math::vec2& GetScale() const;
        double GetRotation() const;

        class State {
        public:
            virtual void Enter(GameObject* object) = 0;
            virtual void Update(GameObject* object, double dt) = 0;
            virtual void CheckExit(GameObject* object) = 0;
            virtual std::string GetName() = 0;
        };
        State* current_state;
        void change_state(State* new_state);

        void SetPosition(Math::vec2 new_position);

        template<typename T>
        T* GetGOComponent() {
            return componentmanager.GetComponent<T>();
        }

        bool Destroyed();

        void UpdatePosition(Math::vec2 delta);
        void SetVelocity(Math::vec2 new_position);
        void UpdateVelocity(Math::vec2 delta);
        void SetScale(Math::vec2 new_scale);
        void UpdateScale(Math::vec2 delta);
        void SetRotation(double new_rotation);
        void UpdateRotation(double delta);
        void Destroy();

        void AddGOComponent(Component* component) {
            componentmanager.AddComponent(component);
        }
        template<typename T>
        void RemoveGOComponent() {
            componentmanager.RemoveComponent<T>();
        }
        void ClearGOComponents() {
            componentmanager.Clear();
        }
        void UpdateGOComponents(double dt) {
            componentmanager.UpdateAll(dt);
        }

    private:
        Math::TransformationMatrix object_matrix;
        bool matrix_outdated;

        double rotation;
        bool destroy;
        Math::vec2 scale;
        Math::vec2 position;
        Math::vec2 velocity;

        class State_None : public State {
        public:
            void Enter(GameObject*) override {}
            void Update(GameObject*, double) override {}
            void CheckExit(GameObject*) override {}
            std::string GetName() { return ""; }
        };
        State_None state_none;
        ComponentManager componentmanager;
    };


}
