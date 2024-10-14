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

void Background::Add(const std::filesystem::path& texture_path, double speed)
{
	backgrounds.push_back(ParallaxLayer({ Engine::GetTextureManager().Load(texture_path), speed }));
}

void Background::Unload()
{
	backgrounds.clear();
}

void Background::Draw(const CS230::Camera& camera)
{
	for (ParallaxLayer& background : backgrounds) {
		//background.texture->Draw(mat3 (( - const_cast<vec2&>(camera.GetPosition()))* (float)background.speed) );
	}
}

ivec2 Background::GetSize()
{
	return backgrounds[backgrounds.size()-1].texture->GetSize();
}
