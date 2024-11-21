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
#include "vec2.h"

#include <iostream>
#include <array>
#define GREEN color3(0,255,0)
#define RED color3(255, 0, 0)

/// /////////////////////////////////////////////////////////////////////////////////////
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
///////////////////////////////////////////////////////////////////////////////////////

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
        transformedPoint.x = (object->GetMatrix() * mat3::build_translation({ boundary.vertices[i] })).column2.x;
        transformedPoint.y = (object->GetMatrix() * mat3::build_translation({ boundary.vertices[i] })).column2.y;
        boundary_poly.vertices.push_back(transformedPoint);
    }

    boundary_poly.vertexCount = (int)boundary_poly.vertices.size();

    return boundary_poly;
}


void CS230::RectCollision::Draw() {
    const float render_height = (float)Engine::window_height;

    Math::rect world_boundary = WorldBoundary_rect();

    vec2 bottom_left = vec2{ world_boundary.Left(), world_boundary.Bottom() };
    vec2 bottom_right = vec2{ world_boundary.Right(), world_boundary.Bottom() };
    vec2 top_left = vec2{ world_boundary.Left(), world_boundary.Top() };
    vec2 top_right = vec2{ world_boundary.Right(), world_boundary.Top() };

    Engine::GetRender().AddDrawCall(top_left, top_right, GREEN);
    Engine::GetRender().AddDrawCall(bottom_right, top_right, GREEN);
    Engine::GetRender().AddDrawCall(bottom_right, bottom_left, GREEN);
    Engine::GetRender().AddDrawCall(top_left, bottom_left, GREEN);
}


bool CS230::RectCollision::IsCollidingWith(GameObject* other_object) {
    Collision* other_collider = other_object->GetGOComponent<Collision>();
    Math::rect rectangle_1 = WorldBoundary_rect();

    if (other_collider == nullptr) {
        return false;
    }

    if (other_collider->Shape() == CollisionShape::Rect) {
        Math::rect rectangle_2 = dynamic_cast<RectCollision*>(other_collider)->WorldBoundary_rect();
        if (rectangle_1.Right() > rectangle_2.Left() &&
            rectangle_1.Left() < rectangle_2.Right() &&
            rectangle_1.Top() > rectangle_2.Bottom() &&
            rectangle_1.Bottom() < rectangle_2.Top()) {
            return true;
        }
        return false;
    }

    if (other_collider->Shape() == CollisionShape::Poly) {
        Polygon other_poly = dynamic_cast<MAP_SATCollision*>(other_collider)->WorldBoundary_poly();

        vec2 rect_vertices[4] = {
            {rectangle_1.Left(), rectangle_1.Bottom()},
            {rectangle_1.Right(), rectangle_1.Bottom()},
            {rectangle_1.Right(), rectangle_1.Top()},
            {rectangle_1.Left(), rectangle_1.Top()}
        };

        float min_distance = std::numeric_limits<float>::max(); 
        int closest_index = -1; 
        bool is_colliding = true; 

        for (int i = 0; i < other_poly.vertexCount; i++) {
            vec2 edge = { other_poly.vertices[(i + 1) % other_poly.vertexCount].x - other_poly.vertices[i].x,
                         other_poly.vertices[(i + 1) % other_poly.vertexCount].y - other_poly.vertices[i].y };
            vec2 axis = NormalizeVector2(GetPerpendicular(edge));

            float minA, maxA;
            ProjectPolygon({ std::vector<vec2>(rect_vertices, rect_vertices + 4), 4 }, axis, minA, maxA);

            float minB, maxB;
            ProjectPolygon(other_poly, axis, minB, maxB);

            if (maxA < minB || maxB < minA) {
                is_colliding = false;  
                break;  
            }

            float distance = std::min(std::abs(maxA - minB), std::abs(maxB - minA));

            if (distance < min_distance) {
                min_distance = distance;
                closest_index = i; 
            }
        }

        if (!is_colliding) {
            return false;
        }

        float expanded_left = rectangle_1.Left() - 15;
        float expanded_right = rectangle_1.Right() + 15;
        float expanded_bottom = rectangle_1.Bottom() - 15;
        float expanded_top = rectangle_1.Top() + 15;

        bool isCornerCollision = false;
        for (const vec2& poly_vertex : other_poly.vertices) {
            bool within_x_bounds = (poly_vertex.x >= expanded_left && poly_vertex.x <= expanded_right);
            bool within_y_bounds = (poly_vertex.y >= expanded_bottom && poly_vertex.y <= expanded_top);

            if (within_x_bounds && within_y_bounds) {
                isCornerCollision = true;
                break;
            }
        }

        if (isCornerCollision && closest_index != -1) {

            vec2 CollidingVertex = other_poly.vertices[closest_index];
            vec2 CollidingSide_1 = other_poly.vertices[(closest_index + 1) % other_poly.vertexCount];
            vec2 CollidingSide_2 = other_poly.vertices[(closest_index - 1 + other_poly.vertexCount) % other_poly.vertexCount];


            vec2 direction1 = CollidingSide_1 - CollidingVertex;
            vec2 direction2 = CollidingSide_2 - CollidingVertex;
            vec2 midpoint_direction = NormalizeVector2(direction1 + direction2);


            colliding_edge = { CollidingVertex, CollidingVertex + midpoint_direction };
            std::cout << "It's Corner" << std::endl;
        }
        else if (!isCornerCollision && closest_index != -1) {
            vec2 CollidingSide_1 = other_poly.vertices[closest_index];
            vec2 CollidingSide_2 = other_poly.vertices[(closest_index + 1) % other_poly.vertexCount];

            colliding_edge = { CollidingSide_1, CollidingSide_2 };
        }

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
        else {
            vec2 point_1 = poly_1.vertices[i];
            vec2 point_2 = poly_1.vertices[(i + 1) % poly_1.vertexCount];
            colliding_edge = { point_1 ,point_2 }; 
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
        else {
            vec2 point_1 = poly_1.vertices[i];
            vec2 point_2 = poly_1.vertices[(i + 1) % poly_1.vertexCount];
            colliding_edge = { point_1 ,point_2 }; 
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
        else {
            vec2 point_1 = poly_1.vertices[i];
            vec2 point_2 = poly_1.vertices[(i + 1) % poly_1.vertexCount];
            colliding_edge = { point_1 ,point_2 };
        }
    }

    return true;
}



void CS230::MAP_SATCollision::Draw() {

    for (int j = 1; j < boundary.vertexCount; ++j) {
        Engine::GetRender().AddDrawCall(vec2{ boundary.vertices[j - 1].x, boundary.vertices[j - 1].y },
            vec2{ boundary.vertices[j].x, boundary.vertices[j].y },
            RED);
    }
    Engine::GetRender().AddDrawCall(vec2{ boundary.vertices.back().x, boundary.vertices.back().y },
        vec2{ boundary.vertices.front().x, boundary.vertices.front().y },
        RED);


}
