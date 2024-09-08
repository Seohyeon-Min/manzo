/*
Copyright (C) 2024 DigiPen Institute of Technology
Reproduction or distribution of this file or its contents without
prior written consent is prohibited
File Name:  Input.h
Project:    CS230 Engine
Author:     Jonathan Holmes, Gyuwon Na
Created:    March 8, 2023
Updated:    May 11, 2024
*/

#pragma once
#include <vector>
#include "Vec2.h"

namespace CS230 {
    class Input {
    public:
        enum class Keys {
            A, B, C, D, E, F, G, H, I, J, K, L, M,
            N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
            Space, Enter, Left, Up, Right, Down, One, Two, Escape, Tab,
            Count
        };

        enum class Button
        {
            Left,Right,Count
        };

        Input();
        void Update();
        bool KeyDown(Keys key);
        bool KeyJustPressed(Keys key);
        bool KeyJustReleased(Keys key);
        void SetKeyDown(Keys key, bool value);

        bool ButtonDown(Button key);
        bool ButtonJustPressed(Button key);
        bool ButtonJustReleased(Button key);
        void SetButtonDown(Button key, bool value);

        Math::vec2 DeltaMouse();
        Math::vec2 MousePosition();

    private:
        std::vector<bool> keys_down;
        std::vector<bool> previous_keys_down;

        std::vector<bool> button_down;
        std::vector<bool> previous_button_down;
    };
}
