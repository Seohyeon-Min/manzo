/*
Copyright (C) 2024 DigiPen Institute of Technology
Reproduction or distribution of this file or its contents without
prior written consent is prohibited
File Name:  Collision.cpp
Project:    CS230 Engine
Author:     Jonathan Holmes, Gyuwon Na
Created:    March 8, 2023
Updated:    May 21, 2024
*/

#include "Collision.h"
#include <rlgl.h>
#include <raylib.h>

CS230::RectCollision::RectCollision(Math::irect boundary, GameObject* object) :
    boundary(boundary),
    object(object)
{ }

Math::rect CS230::RectCollision::WorldBoundary() {
    return {
        object->GetMatrix() * static_cast<Math::vec2>(boundary.point_1),
        object->GetMatrix() * static_cast<Math::vec2>(boundary.point_2)
    };
}

void CS230::RectCollision::Draw(Math::TransformationMatrix display_matrix) {
    const double render_height = rlGetFramebufferHeight();

    Math::rect world_boundary = WorldBoundary();

    Math::vec2 bottom_left = display_matrix * Math::vec2{ static_cast<double>(world_boundary.Left()), static_cast<double>(world_boundary.Bottom()) };
    Math::vec2 bottom_right = display_matrix * Math::vec2{ static_cast<double>(world_boundary.Right()), static_cast<double>(world_boundary.Bottom()) };
    Math::vec2 top_left = display_matrix * Math::vec2{ static_cast<double>(world_boundary.Left()), static_cast<double>(world_boundary.Top()) };
    Math::vec2 top_right = display_matrix * Math::vec2{ static_cast<double>(world_boundary.Right()), static_cast<double>(world_boundary.Top()) };

    bottom_left.y = bottom_left.y * -1 + render_height;
    bottom_right.y = bottom_right.y * -1 + render_height;
    top_left.y = top_left.y * -1 + render_height;
    top_right.y = top_right.y * -1 + render_height;

    DrawLine(int(top_left.x), int(top_left.y), int(top_right.x), int(top_right.y), WHITE);
    DrawLine(int(bottom_right.x), int(bottom_right.y), int(top_right.x), int(top_right.y), WHITE);
    DrawLine(int(bottom_right.x), int(bottom_right.y), int(bottom_left.x), int(bottom_left.y), WHITE);
    DrawLine(int(top_left.x), int(top_left.y), int(bottom_left.x), int(bottom_left.y), WHITE);
}

CS230::CircleCollision::CircleCollision(double radius, GameObject* object) : radius(radius), object(object) { }

void CS230::CircleCollision::Draw(Math::TransformationMatrix display_matrix) {
    const double render_height = rlGetFramebufferHeight();
    Math::vec2 transformed_position = display_matrix * object->GetPosition();
    transformed_position.y = transformed_position.y * -1 + render_height;
    const int num_segments = 36;
    Math::vec2 previous_vertex;
    for (int i = 0; i <= num_segments + 1; i++) {
        double theta = 2.0 * PI * static_cast<double>(i) / static_cast<double>(num_segments);
        Math::vec2 vertex = {
            transformed_position.x + GetRadius() * std::cos(theta),
            transformed_position.y + GetRadius() * std::sin(theta)
        };
        if (i > 0) {
            DrawLine(int(vertex.x), int(vertex.y), int(previous_vertex.x), int(previous_vertex.y), WHITE);
        }
        previous_vertex = vertex;
    }
}

double CS230::CircleCollision::GetRadius()
{
    Math::vec2 scale = object->GetScale();
    return radius * std::max(scale.x, scale.y);
}

bool CS230::RectCollision::IsCollidingWith(GameObject* other_object) {
    Collision* other_collider = other_object->GetGOComponent<Collision>();

    if (!other_collider) {
        return false;
    }

    if (other_collider->Shape() != CollisionShape::Rect) {
        Engine::GetLogger().LogError("Rect vs unsupported type");
        return false;
    }

    Math::rect rectangle_1 = WorldBoundary();
    Math::rect rectangle_2 = dynamic_cast<RectCollision*>(other_collider)->WorldBoundary();

    if (rectangle_1.Left() < rectangle_2.Right() && rectangle_1.Right() > rectangle_2.Left() 
        && rectangle_1.Bottom() < rectangle_2.Top() && rectangle_1.Top() > rectangle_2.Bottom()) {
        return true;
    }
    return false;
}

bool CS230::RectCollision::IsCollidingWith(Math::vec2 point)
{
    Math::rect rectangle = WorldBoundary();

    return point.x >= rectangle.Left() && point.x <= rectangle.Right() &&
        point.y >= rectangle.Bottom() && point.y <= rectangle.Top();
}


bool CS230::CircleCollision::IsCollidingWith(GameObject* other_object) {
    Collision* other_collider = other_object->GetGOComponent<Collision>();

    if (!other_collider) {
        return false;
    }

    if (other_collider->Shape() != CollisionShape::Circle) {
        Engine::GetLogger().LogError("Circle vs unsupported type");
        return false;
    }

    double radius_1 = GetRadius();
    double radius_2 = dynamic_cast<CircleCollision*>(other_collider)->GetRadius();
    Math::vec2 circle_1 = object->GetPosition();
    Math::vec2 circle_2 = dynamic_cast<CircleCollision*>(other_collider)->object->GetPosition();


    if ((circle_1.x - circle_2.x) * (circle_1.x - circle_2.x) + (circle_1.y - circle_2.y) * (circle_1.y - circle_2.y) < (radius_1 + radius_2) * (radius_1 + radius_2)) {
        return true;
    }
    return false;
}

bool CS230::CircleCollision::IsCollidingWith(Math::vec2 point)
{
    Math::vec2 circlePosition = object->GetPosition();
    return (point.x - circlePosition.x) * (point.x - circlePosition.x) + (point.y - circlePosition.y) * (point.y - circlePosition.y) <= GetRadius() * GetRadius(); // Check if squared distance is less than or equal to squared radius
}
