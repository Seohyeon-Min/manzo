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
    //switch (event.type)
    //{

    //}
}

bool Engine::HasGameEnded() {
    if (gamestatemanager.HasGameEnded()) {
        return true;
    }
    return false;
}
