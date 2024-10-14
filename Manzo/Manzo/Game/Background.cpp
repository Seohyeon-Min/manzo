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
Background::Background(vec2 start_position, const CS230::Camera& camera)
    : CS230::GameObject(start_position), camera(camera)  // 카메라 참조로 저장
{
    AddGOComponent(new CS230::Sprite("assets/images/temp_back.spt", this));
}

void Background::Update(double dt)
{
    SetPosition(-camera.GetPosition());
    GameObject::Update(dt);  

    std::cout << "Background position: (" << GetPosition().x << ", " << GetPosition().y << ")" << std::endl;
}

void Background::Draw()
{
    CS230::GameObject::Draw();
}
