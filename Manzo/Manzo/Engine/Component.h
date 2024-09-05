/*
Copyright (C) 2023 DigiPen Institute of Technology
Reproduction or distribution of this file or its contents without
prior written consent is prohibited
File Name:  Component.cpp
Project:    CS230 Engine
Author:     Jonathan Holmes, Seohyeon Min
Created:    March 24, 2023
Updated:    05/08/2024
*/
#pragma once

namespace CS230 {
    class Component {
    public:
        virtual ~Component() {};
        virtual void Update([[maybe_unused]] double dt) {};

    };
}
