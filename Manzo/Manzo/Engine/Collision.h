/*
Copyright (C) 2023 DigiPen Institute of Technology
Reproduction or distribution of this file or its contents without
prior written consent is prohibited
File Name:  Collision.h
Project:    CS230 Engine
Author:     Jonathan Holmes
Created:    March 8, 2023
*/

#include "Rect.h"
#include "Component.h"
#include "color3.h"
#include "GLVertexArray.h"

#pragma once
namespace Math {
    class TransformationMatrix;
}

namespace CS230 {
    class GameObject;

    class Collision : public Component {
    public:
        enum class CollisionShape {
            Rect,
            Circle
        };
        virtual CollisionShape Shape() = 0;
        virtual void Draw() = 0;
        virtual bool IsCollidingWith(GameObject* other_object) = 0;
        virtual bool IsCollidingWith(vec2 point) = 0;
    };

    class RectCollision : public Collision {
    public:
        RectCollision(Math::irect boundary, GameObject* object);
        CollisionShape Shape() override {
            return CollisionShape::Rect;
        }
        void Draw();
        bool IsCollidingWith(GameObject* other_object) override;
        bool IsCollidingWith(vec2 point) override;
        void CreatModel();
        void DrawLine(vec2&, vec2&, color3&);
        Math::rect WorldBoundary();

    private:
        GameObject* object;
        Math::irect boundary;
        GLVertexArray model;
    };
}