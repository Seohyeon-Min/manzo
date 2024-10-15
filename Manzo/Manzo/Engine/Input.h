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
#include "vec2.h"

#include <vector>
#include <SDL2/SDL.h>
#include <array>

namespace CS230 {
    class Input {
    public:
        enum class Keys {
            A = SDL_SCANCODE_A,
            B = SDL_SCANCODE_B,
            C = SDL_SCANCODE_C,
            D = SDL_SCANCODE_D,
            E = SDL_SCANCODE_E,
            F = SDL_SCANCODE_F,
            G = SDL_SCANCODE_G,
            H = SDL_SCANCODE_H,
            I = SDL_SCANCODE_I,
            J = SDL_SCANCODE_J,
            K = SDL_SCANCODE_K,
            L = SDL_SCANCODE_L,
            M = SDL_SCANCODE_M,
            N = SDL_SCANCODE_N,
            O = SDL_SCANCODE_O,
            P = SDL_SCANCODE_P,
            Q = SDL_SCANCODE_Q,
            R = SDL_SCANCODE_R,
            S = SDL_SCANCODE_S,
            T = SDL_SCANCODE_T,
            U = SDL_SCANCODE_U,
            V = SDL_SCANCODE_V,
            W = SDL_SCANCODE_W,
            X = SDL_SCANCODE_X,
            Y = SDL_SCANCODE_Y,
            Z = SDL_SCANCODE_Z,
            Space = SDL_SCANCODE_SPACE,
            Enter = SDL_SCANCODE_RETURN,
            Left = SDL_SCANCODE_LEFT,
            Up = SDL_SCANCODE_UP,
            Right = SDL_SCANCODE_RIGHT,
            Down = SDL_SCANCODE_DOWN,
            Tab = SDL_SCANCODE_TAB,
            Esc = SDL_SCANCODE_ESCAPE,
            Shift = SDL_SCANCODE_LSHIFT,
            Ctrl = SDL_SCANCODE_LCTRL,
            Alt = SDL_SCANCODE_LALT,
            TAB = SDL_SCANCODE_TAB,
            Backspace = SDL_SCANCODE_BACKSPACE,
            Delete = SDL_SCANCODE_DELETE,
            Home = SDL_SCANCODE_HOME,
            End = SDL_SCANCODE_END,
            PageUp = SDL_SCANCODE_PAGEUP,
            PageDown = SDL_SCANCODE_PAGEDOWN,
            Insert = SDL_SCANCODE_INSERT,
            Num0 = SDL_SCANCODE_0,
            Num1 = SDL_SCANCODE_1,
            Num2 = SDL_SCANCODE_2,
            Num3 = SDL_SCANCODE_3,
            Num4 = SDL_SCANCODE_4,
            Num5 = SDL_SCANCODE_5,
            Num6 = SDL_SCANCODE_6,
            Num7 = SDL_SCANCODE_7,
            Num8 = SDL_SCANCODE_8,
            Num9 = SDL_SCANCODE_9,
            F1 = SDL_SCANCODE_F1,
            F2 = SDL_SCANCODE_F2,
            F3 = SDL_SCANCODE_F3,
            F4 = SDL_SCANCODE_F4,
            F5 = SDL_SCANCODE_F5,
            F6 = SDL_SCANCODE_F6,
            F7 = SDL_SCANCODE_F7,
            F8 = SDL_SCANCODE_F8,
            F9 = SDL_SCANCODE_F9,
            F10 = SDL_SCANCODE_F10,
            F11 = SDL_SCANCODE_F11,
            F12 = SDL_SCANCODE_F12,
            Count
        };

        Input();
        void Update();
        void SetKeyDown(Keys key, bool value);
        bool KeyDown(Keys key);
        bool KeyJustPressed(Keys key);
        bool KeyJustReleased(Keys key);

        // 마우스 관련 메소드 추가
        void SetMouseButtonDown(Uint8 button, bool value);
        bool MouseButtonDown(Uint8 button);
        bool MouseButtonJustPressed(Uint8 button);
        bool MouseButtonJustReleased(Uint8 button);
        bool IsMouseMoving();
        // 마우스 포지션을 가져오는 메소드 추가
        vec2 GetMousePosition() const;

    private:
        SDL_Event event;
        const Uint8* current_key_state;
        std::array<bool, SDL_NUM_SCANCODES> previous_keys_down;
        std::array<bool, 6> current_mouse_state; // 0: Left, 1: Middle, 2: Right, 3: X1, 4: X2
        std::array<bool, 6> previous_mouse_state;
        vec2 mouse_position;
        vec2 previous_mouse_position;
        static constexpr int move_factor = 8;
        bool quit = false;
    };
}

constexpr CS230::Input::Keys& operator++(CS230::Input::Keys& key) noexcept;