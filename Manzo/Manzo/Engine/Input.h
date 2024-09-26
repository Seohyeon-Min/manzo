#pragma once
/*
Copyright (C) 2023 DigiPen Institute of Technology
Reproduction or distribution of this file or its contents without
prior written consent is prohibited
File Name:  Input.h
Project:    CS230 Engine
Author:     Jonathan Holmes
Created:    March 8, 2023
*/

#pragma once
#include <vector>
#include <SDL2/SDL.h>

namespace CS230 {
    class Input {
    public:
        enum class Keys {
            A, B, C, D, E, F, G, H, I, J, K, L, M,
            N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
            Space, Enter, Left, Up, Right, Down, Tab,
            Count
        };

        Input();
        void Update();
        void SetKeyDown(Keys key, bool value);
        bool KeyDown(Keys key);
        bool KeyJustPressed(Keys key);
        bool KeyJustReleased(Keys key);

    private:
        SDL_Event event;
        const Uint8* current_key_state = SDL_GetKeyboardState(nullptr);
        Uint8 previous_keys_down[SDL_NUM_SCANCODES] = { 0 };
        bool quit = false;
    };


}

constexpr CS230::Input::Keys& operator++(CS230::Input::Keys& key) noexcept;
