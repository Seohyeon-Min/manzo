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

#include <array>
#define GREEN color3(0,255,0)

CS230::RectCollision::RectCollision(Math::irect boundary, GameObject* object) :
    boundary(boundary),
    object(object)
{
    CreatModel();
}

Math::rect CS230::RectCollision::WorldBoundary() {
    return {
        object->GetMatrix() * vec2(boundary.point_1),
        object->GetMatrix() * vec2(boundary.point_2)    
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

    DrawLine(top_left ,top_right, GREEN);
    DrawLine(bottom_right, top_right, GREEN);
    DrawLine(bottom_right, bottom_left, GREEN);
    DrawLine(top_left, bottom_left, GREEN);
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

void CS230::RectCollision::CreatModel()
{
    float w = 0.5f, h = 0.5f;
    const std::array positions = { vec2{-w, -h}, vec2{w, -h} };

    constexpr auto positions_byte_size = static_cast<long long>(sizeof(vec2) * positions.size());
    constexpr auto buffer_size = positions_byte_size;

    GLVertexBuffer buffer(buffer_size);
    buffer.SetData(std::span(positions));

    // Position attribute
    GLAttributeLayout position;
    position.component_type = GLAttributeLayout::Float;
    position.component_dimension = GLAttributeLayout::_2;
    position.normalized = false;
    position.vertex_layout_location = 0; // Layout location for position
    position.stride = sizeof(vec2); // Stride for position
    position.offset = 0; // Offset for position

    model.AddVertexBuffer(std::move(buffer), { position });
    model.SetPrimitivePattern(GLPrimitive::Lines);
}

void CS230::RectCollision::DrawLine(vec2& start, vec2& end, color3& color)
{
    glCheck(glLineWidth(5.0f));
    std::array<vec2, 2> positions = { start, end };
    // 선을 그리기 위한 버퍼를 설정합니다.
    GLVertexBuffer buffer(sizeof(positions));
    buffer.SetData(std::span(positions));

    // OpenGL 상태 설정
    glUseProgram(shaderProgram); // 사용할 셰이더 프로그램을 설정합니다.
    glUniform3f(colorLocation, color.r, color.g, color.b); // 색상 설정
}
