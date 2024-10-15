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
    : CS230::GameObject(start_position), camera(camera)  // 카메라 참조로 저장
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
    //이거 game object draw(camera)를 주석 빼고 밑에 add draw call을 주석하면 draw함수에서 추가되긴 하는데 그려지지 않음 ㅜㅜ
    //GameObject::Draw(camera_matrix);
    Engine::GetRender().AddDrawCall({
        this->GetGOComponent<CS230::Sprite>()->GetTexture(),
        &camera_matrix, // Pass the camera matrix here
        Engine::GetShaderManager().GetDefaultShader()
        }, DrawLayer::DrawFirst);
}
