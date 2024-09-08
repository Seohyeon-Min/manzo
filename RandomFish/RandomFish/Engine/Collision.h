/*
Copyright (C) 2024 DigiPen Institute of Technology
Reproduction or distribution of this file or its contents without
prior written consent is prohibited
File Name:  Collision.h
Project:    CS230 Engine
Author:     Jonathan Holmes, Gyuwon Na
Created:    March 8, 2023
Updated:    May 13, 2024
*/

#pragma once

#include "Component.h"
#include "GameObject.h"
#include "Rect.h"
#include "Matrix.h"

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
        virtual bool IsCollidingWith(GameObject* other_object) = 0;
        virtual bool IsCollidingWith(Math::vec2 point) = 0;
        virtual CollisionShape Shape() = 0;
        virtual void Draw(Math::TransformationMatrix display_matrix) = 0;
    };

    class RectCollision : public Collision {
    public:
        RectCollision(Math::irect boundary, GameObject* object);
        CollisionShape Shape() override {
            return CollisionShape::Rect;
        }
        bool IsCollidingWith(GameObject* other_object) override;
        bool IsCollidingWith(Math::vec2 point) override;
        void Draw(Math::TransformationMatrix display_matrix) override;
        Math::rect WorldBoundary();
    private:
        GameObject* object;
        Math::irect boundary;
    };

    class CircleCollision : public Collision {
    public:
        CircleCollision(double radius, GameObject* object);
        CollisionShape Shape() override {
            return CollisionShape::Circle;
        }
        bool IsCollidingWith(GameObject* other_object) override;
        bool IsCollidingWith(Math::vec2 point) override;
        void Draw(Math::TransformationMatrix display_matrix) override;
        double GetRadius();
    private:
        GameObject* object;
        double radius;
    };

}
