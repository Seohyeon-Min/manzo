#include "Input.h"
#include "Engine.h"


#include <iostream>

CS230::Input::Input() {
    SDL_Init(SDL_INIT_EVERYTHING);
    current_key_state = SDL_GetKeyboardState(nullptr);
    previous_keys_down.fill(false);
    current_mouse_state.fill(false);
    previous_mouse_state.fill(false);
    mouse_position = { 0, 0 };
}

void CS230::Input::SetKeyDown(Keys key, bool value) {
    previous_keys_down[static_cast<int>(key)] = value;
}

void CS230::Input::Update(const SDL_Event& event) {
    //if (KeyJustPressed(Keys::TAB)) {
    //    std::cout << "Tab key was just pressed!" << std::endl;
    //}
    memcpy(previous_mouse_state.data(), current_mouse_state.data(), current_mouse_state.size() * sizeof(bool));
    for (int i = 0; i < SDL_NUM_SCANCODES; ++i) {
        previous_keys_down[i] = current_key_state[i];
    }

    switch (event.type) {
    case SDL_KEYDOWN:
        SetKeyDown(static_cast<Keys>(event.key.keysym.scancode), true);
        break;
    case SDL_KEYUP:
        SetKeyDown(static_cast<Keys>(event.key.keysym.scancode), false);
        break;
    case SDL_MOUSEBUTTONDOWN:
        SetMouseButtonDown(event.button.button, true);
        break;
    case SDL_MOUSEBUTTONUP:
        SetMouseButtonDown(event.button.button, false);
        break;
    case SDL_MOUSEMOTION:
        mouse_position.x = static_cast<float>(event.motion.x) - (Engine::window_width / 2);
        mouse_position.y = (Engine::window_height / 2) - static_cast<float>(event.motion.y);
        break;
    default:
        break;
    }

    current_key_state = SDL_GetKeyboardState(nullptr);
    Uint32 mouse_state = SDL_GetMouseState(nullptr, nullptr);
    current_mouse_state[0] = mouse_state & SDL_BUTTON(SDL_BUTTON_LEFT);
    current_mouse_state[1] = mouse_state & SDL_BUTTON(SDL_BUTTON_MIDDLE);
    current_mouse_state[2] = mouse_state & SDL_BUTTON(SDL_BUTTON_RIGHT);
    current_mouse_state[3] = mouse_state & SDL_BUTTON(SDL_BUTTON_X1);
    current_mouse_state[4] = mouse_state & SDL_BUTTON(SDL_BUTTON_X2);
}

bool CS230::Input::KeyDown(Keys key) {
    return current_key_state[static_cast<int>(key)];
}

bool CS230::Input::KeyJustPressed(Keys key) {
    if (current_key_state[static_cast<int>(key)] && !previous_keys_down[static_cast<int>(key)])
        std::cout << "asdf" << std::endl;
    return current_key_state[static_cast<int>(key)] && !previous_keys_down[static_cast<int>(key)];
}

bool CS230::Input::KeyJustReleased(Keys key) {
    return !current_key_state[static_cast<int>(key)] && previous_keys_down[static_cast<int>(key)];
}

void CS230::Input::SetMouseButtonDown(Uint8 button, bool value) {
    if (button >= 1 && button <= 5) { // 버튼 범위 체크
        current_mouse_state[button - 1] = value;
    }
}

bool CS230::Input::MouseButtonDown(Uint8 button) {
    return (button >= 1 && button <= 5) ? current_mouse_state[button - 1] : false;
}

bool CS230::Input::MouseButtonJustPressed(Uint8 button) {
    return MouseButtonDown(button) && !previous_mouse_state[button - 1];
}

bool CS230::Input::MouseButtonJustReleased(Uint8 button) {
    return !MouseButtonDown(button) && previous_mouse_state[button - 1];
}

vec2 CS230::Input::GetMousePosition() const {
    return mouse_position;
}