/*
Copyright (C) 2023 DigiPen Institute of Technology
Reproduction or distribution of this file or its contents without
prior written consent is prohibited
File Name:  ShowCollision.h
Project:    CS230 Engine
Author:     Jonathan Holmes
Created:    March 8, 2023
*/

#include "Component.h"
#include "Engine.h"
#pragma once

class ShowCollision : public Component {
public:
    ShowCollision();
    void Update(double dt) override;
    bool Enabled();
private:
    bool enabled;
};

