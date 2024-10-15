/*
Copyright (C) 2023 DigiPen Institute of Technology
Reproduction or distribution of this file or its contents without
prior written consent is prohibited
File Name:  Background.h
Project:    CS230 Engine
Author:     Seohyeon Min
Created:    March 29, 2023
Updated:    March 29, 2023
*/

#include "Background.h"
#include <iostream>
Background::Background(vec2 start_position, CS230::Camera& camera)
    : CS230::GameObject(start_position), camera(camera)  // ī�޶� ������ ����
{
    AddGOComponent(new CS230::Sprite("assets/images/temp_back.spt", this));
}

void Background::Update(double dt)
{
    SetPosition(-camera.GetPosition());
    GameObject::Update(dt);  
}

void Background::Draw(const mat3& camera_matrix) {
    // Retrieve the camera's transformation matrix
    //�̰� game object draw(camera)�� �ּ� ���� �ؿ� add draw call�� �ּ��ϸ� draw�Լ����� �߰��Ǳ� �ϴµ� �׷����� ���� �̤�
    //GameObject::Draw(camera_matrix);
    Engine::GetRender().AddDrawCall({
        this->GetGOComponent<CS230::Sprite>()->GetTexture(),
        &camera_matrix, // Pass the camera matrix here
        Engine::GetShaderManager().GetDefaultShader()
        }, DrawLayer::DrawFirst);
}
