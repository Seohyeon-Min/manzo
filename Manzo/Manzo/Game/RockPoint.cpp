/*
Copyright (C) 2023 DigiPen Institute of Technology
Reproduction or distribution of this file or its contents without
prior written consent is prohibited
File Name:  RockPoint.cpp
Project:    Manzo
Author:     Won Kim
Created:    November 25, 2024
*/

#include "RockPoint.h"

RockPoint::RockPoint(vec2 position) :GameObject({ 0,0 }), position(position)
{
    SetPosition(position);
    AddGOComponent(new CS230::Sprite("assets/images/rockpoint_temp.spt", this));

}

void RockPoint::Update(double dt)
{
    CS230::GameObject::Update(dt);
}

void RockPoint::Draw()
{
    CS230::GameObject::Draw();
}

