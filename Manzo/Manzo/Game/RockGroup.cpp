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

RockGroup::RockGroup(const std::string& index) :GameObject({ 0,0 }), index(index)
{}

void RockGroup::Update(double dt)
{
    CS230::GameObject::Update(dt);
}

void RockGroup::Draw()
{
    CS230::GameObject::Draw();
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
            std::string polyind = (this->index).substr(0, 4);

            if (index == polyind) {
                std::getline(linestream, file_path, ',');
                SetPosition(FindCenterRect());
                AddGOComponent(new CS230::Sprite(file_path, this));

                return true;

            }
        }

    }
    std::cerr << "Index not found in the file." << std::endl;
    return false;
}

vec2 RockGroup::FindCenterRect() {  // Calculate texture's position.
    vec2 center = { 0, 0 };
    vec2 minPoint = rocks[0]->GetPolygon().vertices[0];
    vec2 maxPoint = rocks[0]->GetPolygon().vertices[0];

    for (auto& rock : rocks) {
        Polygon poly = rock->GetPolygon();
        minPoint.x = std::min(minPoint.x, poly.FindBoundary().Left());
        minPoint.y = std::min(minPoint.y, poly.FindBoundary().Bottom());
        maxPoint.x = std::max(maxPoint.x, poly.FindBoundary().Right());
        maxPoint.y = std::max(maxPoint.y, poly.FindBoundary().Top());
    }
    center.x = (minPoint.x + maxPoint.x) / 2;
    center.y = (minPoint.y + maxPoint.y) / 2;
    return center;
}

vec2 RockGroup::FindCenterPoly() {  // Calculate Polygon's position
    vec2 center = { 0, 0 };
    vec2 poly_center = { 0, 0 };

    for (auto& rock : rocks) {
        Polygon poly = rock->GetPolygon();
        poly_center = poly.FindCenter();

        center.x += poly_center.x;
        center.y += poly_center.y;
    }
    center.x /= rocks.size();
    center.y /= rocks.size();

    return center;
}
void RockGroup::SetPoints() {
    for (auto& rock : rocks) {
        Polygon poly = rock->GetPolygon();
        for (int i = 0; i < poly.vertexCount; i++) {
            points.push_back(poly.vertices[i]);
        }
    }
}

bool RockGroup::CanCollideWith(GameObjectTypes other_object)
{
    switch (other_object) {
    case GameObjectTypes::Ship:
        return true;
        break;
    }

    return false;
}

void RockGroup::ResolveCollision(GameObject* other_object)
{
    if (other_object->Type() == GameObjectTypes::Ship) {
        auto* collision_edge = this->GetGOComponent<CS230::RectCollision>();
        if (collision_edge == nullptr) {
            // maybe an error?
        }
        if (can_collide) {
            for (auto& rock : this->GetRocks()) {
                //vec2 direction = rock->GetPosition() - rockpoint->GetPosition();
                vec2 direction = { -1, 0 };
                float speed = 1000;
                rock->Pop(direction, speed);
            }
        }
        can_collide = false;
        

    }
}
