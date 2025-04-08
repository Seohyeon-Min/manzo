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


void Background::Add(const std::filesystem::path& texture_path, float speed, DrawLayer draw_layer)
{
    backgrounds.push_back(ParallaxLayer({ Engine::GetTextureManager().Load(texture_path), speed, mat3{},draw_layer }));
}

void Background::SetUniforms(const GLShader* shader) {
    double currentTime = Engine::GetAudioManager().GetCurrentMusicTime(Engine::GetAudioManager().GetID("assets/audios/bgm_original.wav"));
    //counter += Engine::Instance()->GetDt();
    float texelSizeX = 1.0f / GetSize().width-70.f;
    float texelSizeY = 1.0f / GetSize().height - 70.f;
    shader->SendUniform("uResolution",texelSizeX, texelSizeY);    shader->SendUniform("uBlurDirection", 1.0f, 0.0f); // 가로 방향
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


ivec2 Background::GetSize()
{
    return backgrounds[backgrounds.size() - 1].texture->GetSize();
}
