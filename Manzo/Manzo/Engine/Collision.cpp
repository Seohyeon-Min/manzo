/*
Copyright (C) 2023 DigiPen Institute of Technology
Reproduction or distribution of this file or its contents without
prior written consent is prohibited
File Name:  Collision.cpp
Project:    CS230 Engine
Author:     Jonathan Holmes
Created:    March 8, 2023
*/

#include "Collision.h"
#include "GameObject.h"
#include "color3.h"
#include "ShaderManager.h"
#include "MapManager.h"
#include <iostream>
#include "vec2.h"

#include <array>
#define GREEN color3(0,255,0)

CS230::RectCollision::RectCollision(Math::irect boundary, GameObject* object) :
    boundary(boundary),
    object(object)
{
    Engine::GetShaderManager().LoadShader("default_collision", "assets/shaders/default_collision.vert", "assets/shaders/default_collision.frag");
}

Math::rect CS230::RectCollision::WorldBoundary() {
    std::cout << object->GetMatrix().column2.x << std::endl;
    return {
        {(object->GetMatrix() * mat3::build_translation((vec2)boundary.point_1)).column2.x,(object->GetMatrix() * mat3::build_translation((vec2)boundary.point_1)).column2.y},
        {(object->GetMatrix() * mat3::build_translation((vec2)boundary.point_2)).column2.x,(object->GetMatrix() * mat3::build_translation((vec2)boundary.point_2)).column2.y},
    };
}

void CS230::RectCollision::Draw() {
    const float render_height = (float)Engine::window_height;

    Math::rect world_boundary = WorldBoundary();

    vec2 bottom_left =  vec2{ world_boundary.Left(), world_boundary.Bottom() };
    vec2 bottom_right =  vec2{ world_boundary.Right(), world_boundary.Bottom() };
    vec2 top_left = vec2{ world_boundary.Left(), world_boundary.Top() };
    vec2 top_right =  vec2{ world_boundary.Right(), world_boundary.Top() };

    //bottom_left.y = bottom_left.y * -1 + render_height;
    //bottom_right.y = bottom_right.y * -1 + render_height;
    //top_left.y = top_left.y * -1 + render_height;
    //top_right.y = top_right.y * -1 + render_height;

    Engine::GetRender().AddDrawCall(top_left, top_right, GREEN);
    Engine::GetRender().AddDrawCall(bottom_right, top_right, GREEN);
    Engine::GetRender().AddDrawCall(bottom_right, bottom_left, GREEN);
    Engine::GetRender().AddDrawCall(top_left, bottom_left, GREEN);
}

bool CS230::RectCollision::IsCollidingWith(GameObject* other_object) {
    Collision* other_collider = other_object->GetGOComponent<Collision>();


    if (other_collider == nullptr) {
        return false;
    }


    if (other_collider->Shape() != CollisionShape::Rect) {
        Engine::GetLogger().LogError("Rect vs unsupported type");
        return false;
    }

    Math::rect rectangle_1 = WorldBoundary();
    Math::rect rectangle_2 = dynamic_cast<RectCollision*>(other_collider)->WorldBoundary();

    if (rectangle_1.Right() > rectangle_2.Left() &&
        rectangle_1.Left() < rectangle_2.Right() &&
        rectangle_1.Top() > rectangle_2.Bottom() &&
        rectangle_1.Bottom() < rectangle_2.Top()) {
        return true;
    }
    return false;
}

bool CS230::RectCollision::IsCollidingWith(vec2 point)
{
    Math::rect rectangle_1 = WorldBoundary();

    if (rectangle_1.Right() >= point.x &&
        rectangle_1.Left() <= point.x &&
        rectangle_1.Top() >= point.y &&
        rectangle_1.Bottom() <= point.y) {
        return true;
    }
    return false;
}


//This four funtion is need for SATCollision
vec2 GetPerpendicular(const vec2& v) {
    return { -v.y, v.x };
}
float Vector2DotProduct(const vec2& v1, const vec2& v2) {
    return v1.x * v2.x + v1.y * v2.y;
}
vec2 NormalizeVector2(const vec2& v) {
    float length = sqrt(v.x * v.x + v.y * v.y);
    return { v.x / length, v.y / length };
}
void ProjectPolygon(const Polygon& polygon, const vec2& axis, float& min, float& max) {
    float project_result = Vector2DotProduct(polygon.vertices[0], axis);
    min = project_result;
    max = project_result;

    for (int i = 1; i < polygon.vertexCount; i++) {
        project_result = Vector2DotProduct(polygon.vertices[i], axis);
        if (project_result < min) min = project_result;
        if (project_result > max) max = project_result;
    }
}

bool CS230::MAP_SATCollision::MapCollision(const Polygon& poly1, const Polygon& poly2)
{
    for (int i = 0; i < poly1.vertexCount; i++) {
        vec2 edge = { poly1.vertices[(i + 1) % poly1.vertexCount].x - poly1.vertices[i].x,
                         poly1.vertices[(i + 1) % poly1.vertexCount].y - poly1.vertices[i].y };
        vec2 axis = NormalizeVector2(GetPerpendicular(edge));  

        float minA, maxA;
        ProjectPolygon(poly1, axis, minA, maxA);    

        float minB, maxB;
        ProjectPolygon(poly2, axis, minB, maxB);    

        if (maxA < minB || maxB < minA) {   
            return false;
        }
    }

    for (int i = 0; i < poly2.vertexCount; i++) {
        vec2 edge = { poly2.vertices[(i + 1) % poly2.vertexCount].x - poly2.vertices[i].x,
                         poly2.vertices[(i + 1) % poly2.vertexCount].y - poly2.vertices[i].y };
        vec2 axis = NormalizeVector2(GetPerpendicular(edge));

        float minA, maxA;
        ProjectPolygon(poly1, axis, minA, maxA);

        float minB, maxB;
        ProjectPolygon(poly2, axis, minB, maxB);

        if (maxA < minB || maxB < minA) {
            return false;
        }
    }

    return true;    
}

CS230::MAP_SATCollision::MAP_SATCollision(Polygon boundary, GameObject* object) : 
    boundary(boundary),
    object(object)
{
}

void CS230::MAP_SATCollision::Draw() {

    for (int j = 1; j < boundary.vertexCount; ++j) {
        Engine::GetRender().AddDrawCall(vec2{ boundary.vertices[j - 1].x, boundary.vertices[j - 1].y },
                                        vec2{ boundary.vertices[j].x, boundary.vertices[j].y },
                                        GREEN);
    }
    Engine::GetRender().AddDrawCall(vec2{ boundary.vertices.back().x, boundary.vertices.back().y },
                                    vec2{ boundary.vertices.front().x, boundary.vertices.front().y },
                                    GREEN);


}