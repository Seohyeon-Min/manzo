/*
Copyright (C) 2023 DigiPen Institute of Technology
Reproduction or distribution of this file or its contents without
prior written consent is prohibited
File Name:  Rock.cpp
Project:    Manzo
Author:     Won Kim
Created:    Sept 13, 2024
*/


#include "Rock.h"
#include <iostream>

Rock::Rock(Math::irect boundary): GameObject(static_cast<Math::vec2>(boundary.point_1))
{

	AddGOComponent(new CS230::RectCollision({ Math::ivec2{ 0, 0 }, boundary.Size() }, this));
}
