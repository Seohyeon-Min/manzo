/*
Copyright (C) 2023 DigiPen Institute of Technology
Reproduction or distribution of this file or its contents without
prior written consent is prohibited
File Name:  Input.h
*/

#include "Input.h"
#include "Engine.h"
#include <iostream>


//Engine::GetInput().KeyDown(CS230::Input::Keys::Shift) && Engine::GetInput().KeyDown(CS230::Input::Keys::D) && hero->IsDashReady == true


constexpr CS230::Input::Keys& operator++(CS230::Input::Keys& key) noexcept {
    key = static_cast<CS230::Input::Keys>(static_cast<unsigned>(key) + 1);
    return key;
}



CS230::Input::Input() {
    SDL_Init(SDL_INIT_EVERYTHING);
    current_key_state = SDL_GetKeyboardState(nullptr);
}

void CS230::Input::SetKeyDown(Keys key, bool value) {
    previous_keys_down[static_cast<int>(key)] = value;
}


void CS230::Input::Update() {
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            quit = true;
            break;
        case SDL_KEYDOWN:
            std::cout << "Key pressed: " << SDL_GetKeyName(event.key.keysym.sym) << std::endl;
            SetKeyDown(static_cast<Keys>(event.key.keysym.scancode), true); // 키 상태 업데이트
            break;
        case SDL_KEYUP:
            std::cout << "Key released: " << SDL_GetKeyName(event.key.keysym.sym) << std::endl;
            SetKeyDown(static_cast<Keys>(event.key.keysym.scancode), false); // 키 상태 업데이트
            break;
        default:
            break;
        }
    }

    memcpy(previous_keys_down, current_key_state, SDL_NUM_SCANCODES);
    current_key_state = SDL_GetKeyboardState(nullptr);
}

bool CS230::Input::KeyDown(Keys key) {
    //key와 입력된 값이 같은지 확인해야함
    return current_key_state[static_cast<int>(key)];
}

bool CS230::Input::KeyJustPressed(Keys key) {
    return current_key_state[static_cast<int>(key)] && !previous_keys_down[static_cast<int>(key)];
}

bool CS230::Input::KeyJustReleased(Keys key) {
    return !current_key_state[static_cast<int>(key)] && previous_keys_down[static_cast<int>(key)];
}
