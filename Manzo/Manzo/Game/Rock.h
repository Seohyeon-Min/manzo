/*
Copyright (C) 2023 DigiPen Institute of Technology
Reproduction or distribution of this file or its contents without
prior written consent is prohibited
File Name:  Rock.cpp
Project:    Manzo
Author:     Won Kim
Created:    Sept 13, 2024
*/

#pragma once
#include "..\Engine\GameObject.h"
#include "GameObjectTypes.h"

class Rock : public CS230::GameObject {
public:
    Rock(Math::irect boundary);
    GameObjectTypes Type() override { return GameObjectTypes::Rock; }
    std::string TypeName() override { return "Rock"; }
    const Math::irect GetBoundary() { return boundary; }

    
private:
    Math::irect boundary;
};
