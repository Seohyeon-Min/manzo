/*
Copyright (C) 2023 DigiPen Institute of Technology
Reproduction or distribution of this file or its contents without
prior written consent is prohibited
File Name:  ShowCollision.cpp
Project:    CS230 Engine
Author:     Jonathan Holmes
Created:    March 8, 2023
*/

#include "ShowCollision.h"
#include <iostream>
CS230::ShowCollision::ShowCollision() : enabled(true) {}
//Engine::GetInput().MouseButtonJustPressed(SDL_BUTTON_LEFT)
void CS230::ShowCollision::Update([[maybe_unused]] double dt) {
    if (Engine::GetInput().KeyJustPressed(CS230::Input::Keys::TAB)) {
        enabled = !enabled;
    }
}

bool CS230::ShowCollision::Enabled() {
    return enabled;
}