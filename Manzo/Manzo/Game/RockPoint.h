/*
Copyright (C) 2023 DigiPen Institute of Technology
Reproduction or distribution of this file or its contents without
prior written consent is prohibited
File Name:  RockPoint.h
Project:    Manzo
Author:     Won Kim
Created:    November 25, 2024
*/

#pragma once

#include "vec2.h"
#include "..\Engine\Component.h"
#include "..\Engine\GameObject.h"
#include "..\Engine\Polygon.h"
#include "..\Engine\MapManager.h"
#include "..\Engine\Polygon.h"
#include "GameObjectTypes.h"
#include "RockGroup.h"

#include <vector>
class RockPoint : public CS230::GameObject {
	RockPoint();
	GameObjectTypes Type() override { return GameObjectTypes::RockPoint; }
	std::string TypeName() override { return "Rect"; }
	void Update(double dt);
	void Draw(); 

};
