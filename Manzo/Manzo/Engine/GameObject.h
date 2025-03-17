/*
Copyright (C) 2023 DigiPen Institute of Technology
Reproduction or distribution of this file or its contents without
prior written consent is prohibited
File Name:  GameObject.h
Project:    CS230 Engine
Author:     Jonathan Holmes
Created:    March 8, 2023
*/

#pragma once
#include "Sprite.h"
#include "ComponentManager.h"

#include "vec2.h"
#include "Component.h"
#include "Collision.h"
#include "ShowCollision.h"
#include "engine.h"
#include "GLShader.h"
#include "../Game/GameObjectTypes.h"

#include <string>

namespace Math { class TransformationMatrix; }
enum class GameObjectTypes;


class GameObject : public Component {
public:
    GameObject(vec2 position);
    GameObject(vec2 position, double rotation, vec2 scale);
    virtual ~GameObject() {}

    virtual GameObjectTypes Type() = 0;
    virtual std::string TypeName() = 0;
    bool IsCollidingWith(GameObject* other_object);
    bool IsCollidingWithNextFrame(GameObject* other_object, vec2 velocity, float dt, float& toi);
    bool IsCollidingWith(vec2 point);
    bool IsVisible(const Math::rect& camera_bounds) const;
    bool isCameraFixed() const { return is_camera_fixed; }
    void SetCameraFixed(bool fixed) { is_camera_fixed = fixed; }
    virtual bool CanCollideWith(GameObjectTypes other_object_type);
    virtual void ResolveCollision([[maybe_unused]] GameObject* other_object) { };
    virtual void Update(double dt);
    virtual void FixedUpdate(double fixed_dt);
    virtual void Draw(DrawLayer drawlayer = DrawLayer::Draw);
    virtual void Draw(const DrawCall& draw_call);
    virtual void Destroy() { destroy = true; }
    virtual bool Destroyed() { return destroy; }
    virtual void SetShader(GLShader* DEBUG_NEW_shader) { shader = DEBUG_NEW_shader; };

    const mat3& GetMatrix();
    const Math::rect& GetAABB() const;
    const vec2& GetPosition() const;
    const vec2& GetVelocity() const;
    const vec2& GetScale() const;
    const bool GetFlipX() const;
    //const MAP_SATCollision& GetSATCollision() const;
    double GetRotation() const;
    //void SetModel(GLVertexArray* model) { draw_call.model = model; }

    class State {
    public:
        virtual void Enter(GameObject* object) = 0;
        virtual void Update(GameObject* object, double dt) = 0;
        virtual void FixedUpdate(GameObject* object, double dt) {};
        virtual void CheckExit(GameObject* object) = 0;
        virtual std::string GetName() = 0;
    };
    State* current_state = nullptr;
    void change_state(State* DEBUG_NEW_state);

    void SetPosition(vec2 DEBUG_NEW_position);

    template<typename T>
    T* GetGOComponent() {
        return componentmanager.GetComponent<T>();
    }


    //   protected:
    void UpdatePosition(vec2 delta);
    void SetVelocity(vec2 DEBUG_NEW_velocity);
    void UpdateVelocity(vec2 delta);
    void SetScale(vec2 DEBUG_NEW_scale);
    void SetFlipX(bool flip);
    void UpdateScale(vec2 delta);
    void SetRotation(double DEBUG_NEW_rotation);
    void UpdateRotation(double delta);

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
    ComponentManager componentmanager;
    mat3 object_matrix;

    double rotation;
    bool matrix_outdated = true;
    bool is_camera_fixed = false;
    bool destroy;
    vec2 scale;
    vec2 position;
    vec2 velocity;
    GLShader* shader = nullptr;
    Math::rect cached_aabb;
    vec2 frame_size;

    class State_None : public State {
    public:
        void Enter(GameObject*) override {}
        void Update(GameObject*, double) override {}
        void FixedUpdate(GameObject* object, double dt) override {};
        void CheckExit(GameObject*) override {}
        std::string GetName() { return ""; }
    };
    State_None state_none;
    friend class Sprite;
};
