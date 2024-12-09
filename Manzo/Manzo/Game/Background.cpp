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

void Background::Add(const std::filesystem::path& texture_path, float speed)
{
    backgrounds.push_back(ParallaxLayer({ Engine::GetTextureManager().Load(texture_path), speed }));
}

void Background::SetUniforms(const GLShader* shader) {
    double currentTime = Engine::GetAudioManager().GetCurrentMusicTime(Engine::GetAudioManager().GetID("assets/audios/100BPM_edm_temp.wav"));
    //counter += Engine::Instance().GetDt();
    float texelSizeX = 1.0f / GetSize().width-70.f;
    float texelSizeY = 1.0f / GetSize().height - 70.f;
    shader->SendUniform("uResolution",texelSizeX, texelSizeY);    shader->SendUniform("uBlurDirection", 1.0f, 0.0f); // 가로 방향
    //shader->SendUniform("uChannelResolution", (float)GetSize().width, (float)GetSize().height);
}


void Background::Unload()
{
    backgrounds.clear();
}

void Background::Draw(const CS230::Cam& camera)
{
    vec2 cameraPos = camera.GetPosition();



    for (ParallaxLayer& background : backgrounds) {
        DrawSettings settings;
        settings.do_blending = true;
        settings.is_UI = true;
        // Build the translation matrix with parallax effect
        //background.matrix = mat3::build_translation({ (0 - cameraPos.x) * background.speed, (0 - cameraPos.y) * background.speed });
        //background.matrix = mat3::build_translation({ 0 + cameraPos.x, 0 + cameraPos.y });
        background.matrix = mat3::build_translation({ (0 + cameraPos.x) * background.speed, (0 + cameraPos.y) * background.speed });
        //std::cout << parallax_matrix;
        CS230::DrawCall draw_call = {
            background.texture,                       // Texture to draw
            &background.matrix,                          // Transformation matrix
            Engine::GetShaderManager().GetDefaultShader(),
            nullptr,
            settings
        };

        Engine::GetRender().AddDrawCall(draw_call, DrawLayer::DrawBackground);
    }
}

ivec2 Background::GetSize()
{
    return backgrounds[backgrounds.size() - 1].texture->GetSize();
}
