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

RockGroup::RockGroup(const std::string& index, double rotation, vec2 scale) :GameObject({ 0,0 }, rotation, scale), index(index)
{
    //AddGOComponent(new Sprite("assets/images/rock/g" + index +".spt", this));
}

RockGroup::~RockGroup() {
    rocks.clear();
    moving_rocks.clear();
}

void RockGroup::Update(double dt)
{
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

bool RockGroup::MatchIndex()
{
    std::ifstream file("assets/images/rock/rock.csv");
    std::string line, cell;
    if (!file.is_open()) {
        std::cerr << "Failed to Open CSV." << std::endl;
        return false;
    }

    if (file.is_open()) {
        while (std::getline(file, line)) {
            std::stringstream linestream(line);
            std::string index, x_str, y_str, file_path;

            std::getline(linestream, index, ',');
            std::string polyind = (this->index).substr(0, 5);

            if (index == polyind) {
                std::getline(linestream, file_path, ',');
                SetPosition(FindCenterRect());
                AddGOComponent(new Sprite(file_path, this));

                return true;

            }
        }

    }
    std::cerr << "Index not found in the file." << std::endl;
    return false;
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
