/*
Copyright (C) 2024 DigiPen Institute of Technology
Reproduction or distribution of this file or its contents without
prior written consent is prohibited
File Name:  ScreenWrap.cpp
Project:    CS230 Engine
Author:     Jonathan Holmes, Gyuwon Na
Created:    March 8, 2023
Updated:    May 9, 2024
*/


#include "ScreenWrap.h"

void ScreenWrap::Update([[maybe_unused]] double dt) {
    ivec2 frame_size = object.GetGOComponent<CS230::Sprite>()->GetFrameSize();
    if (object.GetPosition().y > Engine::window_height + frame_size.y / 2.0f) {
        object.SetPosition({ object.GetPosition().x, -Engine::window_height - frame_size.y / 2.0f });
    }
    else if (object.GetPosition().y < -Engine::window_height - frame_size.y / 2.0f) {
        object.SetPosition({ object.GetPosition().x, Engine::window_height + frame_size.y / 2.0f });
    }
    if (object.GetPosition().x > Engine::window_width + object.GetGOComponent<CS230::Sprite>()->GetFrameSize().x / 2.0f) {
        object.SetPosition({ -Engine::window_width - frame_size.x / 2.0f, object.GetPosition().y });
    }
    else if (object.GetPosition().x < -Engine::window_width - frame_size.x / 2.0f) {
        object.SetPosition({ Engine::window_width + frame_size.x / 2.0f, object.GetPosition().y });
    }
}
