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
#include "GLShader.h"
#include "vec2.h"
#include "Polygon.h"

#pragma once
namespace Math {
    class TransformationMatrix;
}


class GameObject;

class Collision : public Component {
public:
    enum class CollisionShape {
        Rect,
        Poly
    };
    virtual CollisionShape Shape() = 0;
    virtual void Draw() = 0;
    virtual bool IsCollidingWith(GameObject* other_object) = 0;
    virtual bool IsCollidingWith(vec2 point) = 0;
    virtual Math::rect GetBoundary() const = 0;
};

class RectCollision : public Collision {
public:
    RectCollision(Math::irect boundary, GameObject* object);
    CollisionShape Shape() override {
        return CollisionShape::Rect;
    }
    void Draw();
    bool IsCollidingWith(GameObject* other_object) override;
    bool IsCollidingWith(Collision* other_collider, Math::rect next_rect);
    bool IsCollidingWithNextFrame(GameObject* other_object, vec2 velocity, float dt, float& toi);
    bool IsCollidingWith(vec2 point) override;
    Math::rect GetBoundary() const override {
        return WorldBoundary_rect();
    }
    Math::rect WorldBoundary_rect() const;
    std::pair<vec2, vec2> GetCollidingEdge() { return colliding_edge; }

private:
    GameObject* object;
    Math::irect boundary;
    std::pair<vec2, vec2> colliding_edge{};
};

class MAP_SATCollision : public Collision {
public:
    MAP_SATCollision(Polygon boundary, GameObject* object);
    void Draw() override;
    CollisionShape Shape() override {
        return CollisionShape::Poly;
    }
    bool IsCollidingWith(GameObject* other_object) override;
    bool IsCollidingWith(vec2 point) override;
    Math::rect GetBoundary() const override {
        return Math::rect{};
    }
    Polygon WorldBoundary_poly() const;
    std::pair<vec2, vec2> GetCollidingEdge() { return colliding_edge; }
private:
    GameObject* object;
    Polygon boundary;
    std::pair<vec2, vec2> colliding_edge{};
};
