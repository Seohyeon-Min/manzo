/*
Copyright (C) 2023 DigiPen Institute of Technology
Reproduction or distribution of this file or its contents without
prior written consent is prohibited
File Name:  Engine.cpp
Project:    CS230 Engine
Author:     Jonathan Holmes
Created:    March 8, 2023
*/

#include "Engine.h"
#include <SDL2/SDL.h>
#include <iostream>


Engine::Engine() :
#ifdef _DEBUG
    logger(last_tick, CS230::Logger::Severity::Debug, true)
#else
    logger(CS230::Logger::Severity::Event, false)
#endif
{
    unsigned int seed = static_cast<unsigned int>(std::time(NULL));
    std::srand(seed);
    logger.LogEvent("Random seed :" + std::to_string(seed));
    logger.LogEvent("Engine Started");
    //window.Start(window_title);
    //Start other services
    last_test = last_tick;
}

void Engine::Stop() {
    //Stop all services
    logger.LogEvent("Engine Stopped");
}

void Engine::Update() {

    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    double dt = std::chrono::duration<double>(now - last_tick).count();

    if (dt > (1 / TargetFPS))
    {
        frame_count++;
        logger.LogVerbose("Engine Update");
        last_tick = now;

        gamestatemanager.Update(dt);
        input.Update();
    }

    if (frame_count >= FPSTargetFrames) {
        double actual_time = std::chrono::duration<double>(now - last_test).count();
        logger.LogDebug("FPS: " + std::to_string(frame_count / actual_time));
        frame_count = 0;
        last_test = now;
    }
}

void Engine::HandleEvent(SDL_Window& sdl_window, const SDL_Event& event)
{
    switch (event.type)
    {
    case SDL_QUIT << '\n':
        break;
    case SDL_MOUSEWHEEL:
        std::clog << "Mouse Wheel (" << event.wheel.preciseX << ", " << event.wheel.preciseY << ")\n";
        break;
    case SDL_MOUSEBUTTONDOWN:
    case SDL_MOUSEBUTTONUP:
    {
        const char* button_text = "";
        switch (event.button.button)
        {
        case SDL_BUTTON_LEFT:
            button_text = "Left";
            break;
        case SDL_BUTTON_MIDDLE:
            button_text = "Middle";
            break;
        case SDL_BUTTON_RIGHT:
            button_text = "Right";
            break;
        default: // SDL_BUTTON_X1, SDL_BUTTON_X2
            button_text = "Extra Button";
            break;
        }
        const char* pressed_state = "";
        if (event.key.state == SDL_PRESSED)
            pressed_state = "Pressed";
        else // SDL_RELEASED
            pressed_state = "Released";
        const char* click = "";
        if (event.button.clicks == 1)
            click = "Single Click ";
        else if (event.button.clicks == 2)
            click = "Double Click ";
        std::clog << click << pressed_state << " on " << button_text << " at (" << event.button.x << "," << event.button.y << ")\n";
    }
    break;
    case SDL_WINDOWEVENT:
    {
        switch (event.window.event)
        {
        case SDL_WINDOWEVENT_SHOWN:
            std::clog << "Window " << event.window.windowID << " shown" << '\n';
            break;
        case SDL_WINDOWEVENT_HIDDEN:
            std::clog << "Window " << event.window.windowID << " hidden" << '\n';
            break;
        case SDL_WINDOWEVENT_EXPOSED:
            std::clog << "Window " << event.window.windowID << " exposed" << '\n';
            break;
        case SDL_WINDOWEVENT_MOVED:
            std::clog << "Window " << event.window.windowID << " moved to " << event.window.data1 << ',' << event.window.data2 << '\n';
            break;
        case SDL_WINDOWEVENT_MINIMIZED:
            std::clog << "Window " << event.window.windowID << " minimized" << '\n';
            break;
        case SDL_WINDOWEVENT_MAXIMIZED:
            std::clog << "Window " << event.window.windowID << " maximized" << '\n';
            break;
        case SDL_WINDOWEVENT_RESTORED:
            std::clog << "Window " << event.window.windowID << " restored" << '\n';
            break;
        case SDL_WINDOWEVENT_ENTER:
            std::clog << "Mouse entered window " << event.window.windowID << '\n';
            break;
        case SDL_WINDOWEVENT_LEAVE:
            std::clog << "Mouse left window " << event.window.windowID << '\n';
            break;
        case SDL_WINDOWEVENT_FOCUS_GAINED:
            std::clog << "Window " << event.window.windowID << " gained keyboard focus" << '\n';
            break;
        case SDL_WINDOWEVENT_FOCUS_LOST:
            std::clog << "Window " << event.window.windowID << " lost keyboard focus" << '\n';
            break;
        case SDL_WINDOWEVENT_CLOSE:
            std::clog << "Window " << event.window.windowID << " closed" << '\n';
            break;
        case SDL_WINDOWEVENT_TAKE_FOCUS:
            std::clog << "Window " << event.window.windowID << " is offered a focus" << '\n';
            break;
        case SDL_WINDOWEVENT_HIT_TEST:
            std::clog << "Window " << event.window.windowID << " has a special hit test" << '\n';
            break;
        default:
            std::clog << "Window " << event.window.windowID << " got unknown event " << event.window.event << '\n';
            break;
        }
    }
    }

}

bool Engine::HasGameEnded() {
    if (gamestatemanager.HasGameEnded()) {
        return true;
    }
    return false;
}
