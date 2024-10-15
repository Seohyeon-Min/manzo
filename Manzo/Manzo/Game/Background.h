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
#include "GameObjectTypes.h"

class Background : public CS230::GameObject {
public:
    Background(vec2 start_position);
    GameObjectTypes Type() override { return GameObjectTypes::Background; }
    std::string TypeName() override { return "Background"; }
    void Draw();
    ivec2 GetSize();
private:
    struct ParallaxLayer {
        GLTexture* texture;
        float speed = 1.f;
    };
    CS230::DrawCall draw_call;
    std::vector<ParallaxLayer> backgrounds;
};
