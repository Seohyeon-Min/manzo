/*
Copyright (C) 2023 DigiPen Institute of Technology
Reproduction or distribution of this file or its contents without
prior written consent is prohibited
File Name:  Gravity.h
Project:    CS230 Engine
Author:     Jonathan Holmes
Created:    Aprill 30, 2023
*/


#include "..\Engine\Component.h"

#pragma once
class Gravity : public CS230::Component {
public:
    Gravity(double value) : gravity(value) {}
    double GetValue() { return gravity; }
private:
    double gravity;
};
