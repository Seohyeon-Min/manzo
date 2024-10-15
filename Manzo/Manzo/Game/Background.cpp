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

Background::Background(vec2 start_position) : GameObject(start_position)
{
    AddGOComponent(new CS230::Sprite("assets/images/temp_back.spt", this));
}

void Background::Draw()
{
    GameObject::Draw();
}

ivec2 Background::GetSize()
{
	return backgrounds[backgrounds.size()-1].texture->GetSize();;
}
