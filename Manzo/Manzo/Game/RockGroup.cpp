/*
Copyright (C) 2023 DigiPen Institute of Technology
Reproduction or distribution of this file or its contents without
prior written consent is prohibited
File Name:  RockGroup.cpp
Project:    Manzo
Author:     Won Kim
Created:    November 25, 2024
*/

#include "RockGroup.h"

RockGroup::RockGroup(const std::string& index, const std::string& map_index, double rotation, vec2 scale) :GameObject({ 0,0 }, rotation, scale), index(index), map_index(map_index)
{
    std::string spritePath = "assets/images/rock/level1/" + index + ".spt";
    std::cout << spritePath << std::endl;
    if (std::filesystem::exists(spritePath)) {
        AddGOComponent(new Sprite(spritePath, this));
    }
    else {
        std::cout << "Warning: Sprite file not found: " << spritePath << std::endl;

    }
    
}

RockGroup::~RockGroup() {
    rocks.clear();
    moving_rocks.clear();
}

void RockGroup::Update(double dt) {
    GameObject::Update(dt);

}


void RockGroup::Draw()
{
    GameObject::Draw(DrawLayer::DrawLast);
}

Math::rect RockGroup::FindBoundary() {
    vec2 minPoint;
    vec2 maxPoint;


    if (!rocks.empty()) {
        for (auto& rock : rocks) {
            Polygon poly = rock->GetOriginalPoly();
            if (poly.vertices.empty()) {
                return Math::rect{};
            }
            minPoint = poly.vertices[0];
            maxPoint = poly.vertices[0];
            for (const auto& vertex : poly.vertices) {
                minPoint.x = std::min(minPoint.x, vertex.x);
                minPoint.y = std::min(minPoint.y, vertex.y);
                maxPoint.x = std::max(maxPoint.x, vertex.x);
                maxPoint.y = std::max(maxPoint.y, vertex.y);
            }

            
        }
        return Math::rect{ minPoint, maxPoint };
    }
    else {
        return Math::rect{};
    }
}


vec2 RockGroup::FindCenterRect() {  // Calculate texture's position.
    vec2 center = { 0, 0 };
    vec2 minPoint;
    vec2 maxPoint;
    if (!rocks.empty()) {

        minPoint = rocks[0]->GetOriginalPoly().vertices[0];
        maxPoint = rocks[0]->GetOriginalPoly().vertices[0];

        for (auto& rock : rocks) {
            Polygon poly = rock->GetOriginalPoly();
            minPoint.x = std::min(minPoint.x, poly.FindBoundary().Left());
            minPoint.y = std::min(minPoint.y, poly.FindBoundary().Bottom());
            maxPoint.x = std::max(maxPoint.x, poly.FindBoundary().Right());
            maxPoint.y = std::max(maxPoint.y, poly.FindBoundary().Top());
        }
    }
    else if (!moving_rocks.empty()) {

        minPoint = moving_rocks[0]->GetOriginalPoly().vertices[0];
        maxPoint = moving_rocks[0]->GetOriginalPoly().vertices[0];

        for (MovingRock* rock : moving_rocks) {
            Polygon poly = rock->GetOriginalPoly();
            minPoint.x = std::min(minPoint.x, poly.FindBoundary().Left());
            minPoint.y = std::min(minPoint.y, poly.FindBoundary().Bottom());
            maxPoint.x = std::max(maxPoint.x, poly.FindBoundary().Right());
            maxPoint.y = std::max(maxPoint.y, poly.FindBoundary().Top());
        }
    }

    
    center.x = (minPoint.x + maxPoint.x) / 2;
    center.y = (minPoint.y + maxPoint.y) / 2;
    this->start_position = center;
    return center;
}

vec2 RockGroup::FindCenterPoly() {  // Calculate Polygon's position
    vec2 center = { 0, 0 };
    vec2 poly_center = { 0, 0 };

    if (!rocks.empty()) {
        for (auto& rock : rocks) {
            Polygon poly = rock->GetOriginalPoly();
            poly_center = poly.FindCenter();

            center.x += poly_center.x;
            center.y += poly_center.y;
        }

        center.x /= rocks.size();
        center.y /= rocks.size();
    }
    else if (!moving_rocks.empty()) {
        for (MovingRock* rock : moving_rocks) {
            Polygon poly = rock->GetOriginalPoly();
            poly_center = poly.FindCenter();

            center.x += poly_center.x;
            center.y += poly_center.y;
        }

        center.x /= moving_rocks.size();
        center.y /= moving_rocks.size();
    }

    
    return center;
}

void RockGroup::SetPoints() {
    if (!rocks.empty()) {
        for (auto& rock : rocks) {
            Polygon poly = rock->GetOriginalPoly();
            for (int i = 0; i < poly.vertexCount; i++) {
                points.push_back(poly.vertices[i]);
            }
        }
    }
    else if (!moving_rocks.empty()) {
        for (MovingRock* rock : moving_rocks) {
            Polygon poly = rock->GetOriginalPoly();
            for (int i = 0; i < poly.vertexCount; i++) {
                points.push_back(poly.vertices[i]);
            }
        }
    }
}

bool RockGroup::CanCollideWith(GameObjectTypes other_object)
{
    /*switch (other_object) {
    case GameObjectTypes::Ship:
        return true;
        break;
    }*/

    return false;
}

void RockGroup::ResolveCollision(GameObject* other_object)
{
    if (other_object->Type() == GameObjectTypes::Ship) {
        auto* collision_edge = this->GetGOComponent<RectCollision>();
        if (collision_edge == nullptr) {
            // maybe an error?
        }
        if (this->can_collide) {
            for (MovingRock* rock : this->GetMovingRocks()) {
                rock->Hit(true);    // rock changes state to State_Pop

                this->can_collide = false;
            }
        }
        

    }
}
