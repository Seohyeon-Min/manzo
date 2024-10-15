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

Math::rect CS230::RectCollision::WorldBoundary_rect() {
    //std::cout << object->GetMatrix().column2.x << std::endl;
    return {
        {(object->GetMatrix() * mat3::build_translation((vec2)boundary.point_1)).column2.x,
         (object->GetMatrix() * mat3::build_translation((vec2)boundary.point_1)).column2.y},
        {(object->GetMatrix() * mat3::build_translation((vec2)boundary.point_2)).column2.x,
         (object->GetMatrix() * mat3::build_translation((vec2)boundary.point_2)).column2.y},
    };
}

Polygon CS230::MAP_SATCollision::WorldBoundary_poly() {
    Polygon boundary_poly;
    vec2 transformedPoint;

    for (int i = 0; i < boundary.vertexCount; ++i) {
        transformedPoint.x = (object->GetMatrix() * mat3::build_translation({ boundary.vertices[i].x, 0 })).column2.y;
        transformedPoint.y = (object->GetMatrix() * mat3::build_translation({ 0, boundary.vertices[i].y })).column2.x;
        boundary_poly.vertices.push_back(transformedPoint);
    }

    boundary_poly.vertexCount = (int)boundary_poly.vertices.size();  

    return boundary_poly;
}

void CS230::RectCollision::Draw() {
    const float render_height = (float)Engine::window_height;

    Math::rect world_boundary = WorldBoundary_rect();

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

    Math::rect rectangle_1 = WorldBoundary_rect();
    Math::rect rectangle_2 = dynamic_cast<RectCollision*>(other_collider)->WorldBoundary_rect();

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
    Math::rect rectangle_1 = WorldBoundary_rect();

    if (rectangle_1.Right() >= point.x &&
        rectangle_1.Left() <= point.x &&
        rectangle_1.Top() >= point.y &&
        rectangle_1.Bottom() <= point.y) {
        return true;
    }
    return false;
}



CS230::MAP_SATCollision::MAP_SATCollision(Polygon boundary, GameObject* object) :
    boundary(boundary),
    object(object)
{
}


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
bool CS230::RectCollision::IsCollidingWith(Polygon map_object)
{
    Math::rect rect = WorldBoundary_rect();

    vec2 rect_vertices[4] = {
        {rect.Left(), rect.Bottom()},
        {rect.Right(), rect.Bottom()},
        {rect.Right(), rect.Top()},
        {rect.Left(), rect.Top()}
    };

    for (int i = 0; i < 4; i++) {
        vec2 edge = { rect_vertices[(i + 1) % 4].x - rect_vertices[i].x,
                      rect_vertices[(i + 1) % 4].y - rect_vertices[i].y };
        vec2 axis = NormalizeVector2(GetPerpendicular(edge)); 

        float minA, maxA;
        ProjectPolygon({ std::vector<vec2>(rect_vertices, rect_vertices + 4), 4 }, axis, minA, maxA);
        float minB, maxB;
        ProjectPolygon(map_object, axis, minB, maxB);

        if (maxA < minB || maxB < minA) {
            return false; 
        }
    }
    for (int i = 0; i < map_object.vertexCount; i++) {
        vec2 edge = { map_object.vertices[(i + 1) % map_object.vertexCount].x - map_object.vertices[i].x,
                      map_object.vertices[(i + 1) % map_object.vertexCount].y - map_object.vertices[i].y };
        vec2 axis = NormalizeVector2(GetPerpendicular(edge));  

        float minA, maxA;
        ProjectPolygon({ std::vector<vec2>(rect_vertices, rect_vertices + 4), 4 }, axis, minA, maxA);

        float minB, maxB;
        ProjectPolygon(map_object, axis, minB, maxB);

        if (maxA < minB || maxB < minA) {
            return false;  
        }
    }

    return true;  
}



bool CS230::MAP_SATCollision::IsCollidingWith(vec2 point) {
    Polygon poly_1 = WorldBoundary_poly();
    for (int i = 0; i < poly_1.vertexCount; i++) {
        vec2 edge = { poly_1.vertices[(i + 1) % poly_1.vertexCount].x - poly_1.vertices[i].x,
                      poly_1.vertices[(i + 1) % poly_1.vertexCount].y - poly_1.vertices[i].y };
        vec2 axis = NormalizeVector2(GetPerpendicular(edge));  

        float minA, maxA;
        ProjectPolygon(poly_1, axis, minA, maxA);

        float projection = Vector2DotProduct(point, axis);  
        float minB = projection;
        float maxB = projection;
        if (maxA < minB || maxB < minA) {
            return false;
        }
    }

    return true; 
}

bool CS230::MAP_SATCollision::IsCollidingWith(GameObject* other_object)
{
    Collision* other_collider = other_object->GetGOComponent<Collision>();


    if (other_collider == nullptr) {
        return false;
    }


    if (other_collider->Shape() != CollisionShape::Circle) {
        Engine::GetLogger().LogError("Circle type is cannot apply in Map Collision");
        return false;
    }

    Polygon poly_1 = WorldBoundary_poly();
    Polygon poly_2 = dynamic_cast<MAP_SATCollision*>(other_collider)->WorldBoundary_poly();


    for (int i = 0; i < poly_1.vertexCount; i++) {
        vec2 edge = { poly_1.vertices[(i + 1) % poly_1.vertexCount].x - poly_1.vertices[i].x,
                         poly_1.vertices[(i + 1) % poly_1.vertexCount].y - poly_1.vertices[i].y };
        vec2 axis = NormalizeVector2(GetPerpendicular(edge));  

        float minA, maxA;
        ProjectPolygon(poly_1, axis, minA, maxA);

        float minB, maxB;
        ProjectPolygon(poly_2, axis, minB, maxB);

        if (maxA < minB || maxB < minA) {   
            return false;
        }
    }

    

    for (int i = 0; i < poly_2.vertexCount; i++) {
        vec2 edge = { poly_2.vertices[(i + 1) % poly_2.vertexCount].x - poly_2.vertices[i].x,
                         poly_2.vertices[(i + 1) % poly_2.vertexCount].y - poly_2.vertices[i].y };
        vec2 axis = NormalizeVector2(GetPerpendicular(edge));

        float minA, maxA;
        ProjectPolygon(poly_1, axis, minA, maxA);

        float minB, maxB;
        ProjectPolygon(poly_2, axis, minB, maxB);

        if (maxA < minB || maxB < minA) {
            return false;
        }
    }

    return true;    
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