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

RockPoint::RockPoint() :GameObject({ 0,0 })
{
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

//bool RockPoint::CanCollideWith(GameObjectTypes other_object)
//{
//    switch (other_object) {
//    case GameObjectTypes::Rock:
//        return true;
//        break;
//    }
//
//    return false;
//}
//
//void RockPoint::ResolveCollision(GameObject* other_object)
//{
//}
