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
#include "Ship.h"

class Background : public Component {
public:
    Background();
    void Add(const std::filesystem::path& texture_path, float speed, DrawLayer = DrawLayer::DrawBackground);
    void Unload();
    void Draw(const Cam& camera);
    void ShaderBackgroundDraw(GLShader* shader, const Cam& camera, Ship* ship, std::function<void(const GLShader*)> SetUniformsFunc = nullptr);
    void SetUniforms(const GLShader* shader, Ship* ship);
    ivec2 GetSize();
private:
    struct ParallaxLayer {
        GLTexture* texture;
        float speed = 1.f;
        mat3 matrix;
        DrawLayer drawlayer;
    };
    std::vector<ParallaxLayer> backgrounds;
    GLTexture* full_quad;
    mat3 basic_mat;
};