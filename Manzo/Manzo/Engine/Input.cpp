#include "Input.h"
#include "Engine.h"
#include "../Game/Particles.h"

#include "Particle.h"
#include "..\Engine\Camera.h"

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

void CS230::Input::Update() {
    memcpy(previous_mouse_state.data(), current_mouse_state.data(), current_mouse_state.size() * sizeof(bool));
    for (int i = 0; i < SDL_NUM_SCANCODES; ++i) {
        previous_keys_down[i] = current_key_state[i];
    }
    previous_mouse_position = mouse_position;

    int x, y;

    current_key_state = SDL_GetKeyboardState(nullptr);
    Uint32 mouse_state = SDL_GetMouseState(&x, &y);

    mouse_position.x = static_cast<float>(x);
    mouse_position.y = Engine::window_height - static_cast<float>(y);
    compute_mouse_coordinates();

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
    return current_key_state[static_cast<int>(key)] && !previous_keys_down[static_cast<int>(key)];
}

bool CS230::Input::KeyJustReleased(Keys key) {
    return !current_key_state[static_cast<int>(key)] && previous_keys_down[static_cast<int>(key)];
}

void CS230::Input::compute_mouse_coordinates()
{
    if (!Engine::GetGameStateManager().IsNull() && Engine::GetGameStateManager().GetGSComponent<CS230::Cam>() != nullptr) {
        CS230::Cam* camera = Engine::GetGameStateManager().GetGSComponent<CS230::Cam>();
        // use camera view buildwindowdevicetocamera to convert to camera space
        mat3 device_mouse = mat3::build_translation({ mouse_position });

        mat3 cam_mouse = camera->GetCameraView().BuildWindowDeviceToCamera() * device_mouse;
        // save to environment.mousecamspacex/environment.mousecamspacey
        environment.mouseCamSpaceX = cam_mouse.column2.x;
        environment.mouseCamSpaceY = cam_mouse.column2.y;

        // use camera buildcameratoworld to convert to world space
        mat3 world_mouse = device_mouse * camera->GetCamera().BuildCameraToWorld();
        // save to environment.mouseworldspacex/environment.mouseworldspacey
        environment.mouseWorldSpaceX = world_mouse.column2.x;
        environment.mouseWorldSpaceY = world_mouse.column2.y;
    }
}



void CS230::Input::SetMouseButtonDown(Uint8 button, bool value) {
    if (button >= 1 && button <= 5) { 
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

bool CS230::Input::IsMouseMoving() {
    return (std::abs(mouse_position.x - previous_mouse_position.x) >= move_factor ||
        std::abs(mouse_position.y - previous_mouse_position.y) >= move_factor);
}