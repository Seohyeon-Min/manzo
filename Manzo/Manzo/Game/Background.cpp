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

void Background::Add(const std::filesystem::path& texture_path, float speed)
{
	backgrounds.push_back(ParallaxLayer({ Engine::GetTextureManager().Load(texture_path), speed }));
}

void Background::Unload()
{
	backgrounds.clear();
}

void Background::Draw(const CS230::Cam& camera)
{
    vec2 cameraPos = camera.GetPosition();

    for (ParallaxLayer& background : backgrounds) {
   
        // Build the translation matrix with parallax effect
        mat3 parallax_matrix = mat3::build_translation({ (0 - cameraPos.x) * background.speed, (0 - cameraPos.y) * background.speed });

        draw_call = {
            background.texture,                       // Texture to draw
            &parallax_matrix,                          // Transformation matrix
            Engine::GetShaderManager().GetDefaultShader() // Shader to use
        };

        // Add the draw call to the renderer
        Engine::GetRender().AddDrawCall(draw_call, DrawLayer::DrawFirst);
    }// Somewhere in your main game loop or rendering function
    Engine::GetRender().DrawBackground(draw_call);
}

ivec2 Background::GetSize()
{
	return backgrounds[backgrounds.size()-1].texture->GetSize();;
}
