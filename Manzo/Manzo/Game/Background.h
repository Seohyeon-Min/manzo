/*
Copyright (C) 2023 DigiPen Institute of Technology
Reproduction or distribution of this file or its contents without
prior written consent is prohibited
File Name:  Background.h
Project:    CS230 Engine
Author:     Jonathan Holmes
Created:    March 8, 2023
*/

#pragma once
#include "GLTexture.h"
#include "../Engine/Camera.h"
#include "../Engine/Engine.h"
#include "../Engine/Component.h"
#include "../Engine/GameObject.h"
#include "GameObjectTypes.h"

class Background : public CS230::GameObject {
public:
    Background(vec2 start_position, const CS230::Camera& camera);  // 카메라를 참조로 받음
    GameObjectTypes Type() override { return GameObjectTypes::Background; }
    std::string TypeName() override { return "Background"; }

    void Update(double dt);
    void Draw();

private:
    const CS230::Camera& camera;  // 참조로 저장
};
