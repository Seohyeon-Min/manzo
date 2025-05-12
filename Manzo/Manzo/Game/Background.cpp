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


Background::Background()
{
    full_quad = Engine::GetTextureManager().Load("assets/images/effect/full_quad.png");
}

void Background::Add(const std::filesystem::path& texture_path, float speed, DrawLayer draw_layer)
{
    backgrounds.push_back(ParallaxLayer({ Engine::GetTextureManager().Load(texture_path), speed, mat3{},draw_layer }));
}

void Background::SetUniforms(const GLShader* shader, Ship* ship) {
    float how_deepY = 5670.f;
    float pos_Y = how_deepY + ship->GetPosition().y + 200.f;
    shader->SendUniform("u_y_deep", how_deepY);
    shader->SendUniform("u_y_position", pos_Y);
    shader->SendUniform("u_resolution", Engine::window_width, Engine::window_height); // 가로 방향
    //shader->SendUniform("uChannelResolution", (float)GetSize().width, (float)GetSize().height);
}


void Background::Unload()
{
    backgrounds.clear();
}

void Background::Draw(const Cam& camera)
{
    vec2 cameraPos = camera.GetPosition();

    for (ParallaxLayer& background : backgrounds) {

        background.matrix = mat3::build_translation({ (0 + cameraPos.x) * background.speed, (0 + cameraPos.y) * background.speed });

        DrawCall draw_call = {
            background.texture,                       // Texture to draw
            &background.matrix,                          // Transformation matrix
            Engine::GetShaderManager().GetDefaultShader()
        };

        draw_call.settings.do_blending = true;
        draw_call.settings.is_camera_fixed = true;

        Engine::GetRender().AddBackgroundDrawCall(draw_call);
    }
}

void Background::ShaderBackgroundDraw(GLShader* shader, const Cam& camera, Ship* ship)
{
    vec2 cameraPos = camera.GetPosition();
    basic_mat = mat3::build_translation({ (0 + cameraPos.x), (0 + cameraPos.y) });

    DrawCall draw_call = {
        full_quad,
        &basic_mat,
        shader
    };

    draw_call.settings.do_blending = true;
    draw_call.settings.is_camera_fixed = true;
    draw_call.SetUniforms = [this, ship](const GLShader* shader) { SetUniforms(shader, ship); };

    Engine::GetRender().AddBackgroundDrawCall(draw_call);
}

ivec2 Background::GetSize()
{
    return backgrounds[backgrounds.size() - 1].texture->GetSize();
}