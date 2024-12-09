/*
Copyright (C) 2024 DigiPen Institute of Technology
Reproduction or distribution of this file or its contents without
prior written consent is prohibited
File Name:  ScreenWrap.h
Project:    CS230 Engine
Author:     Jonathan Holmes, Gyuwon Na
Created:    March 8, 2023
Updated:    May 9, 2024
*/


#pragma once
#include "../Engine/Component.h"
#include "../Engine/GameObject.h"

class ScreenWrap : public Component {
public:
    ScreenWrap(GameObject& object) : object(object) {}
    void Update(double dt) override;
private:
    GameObject& object;
};
