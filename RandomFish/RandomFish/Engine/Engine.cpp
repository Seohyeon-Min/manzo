
/*
Copyright (C) 2024 DigiPen Institute of Technology
Reproduction or distribution of this file or its contents without
prior written consent is prohibited
File Name:  Engine.cpp
Project:    CS230 Engine
Author:     Jonathan Holmes, Gyuwon Na
Created:    March 8, 2023
Updated:    April 30, 2024
*/

#include "Engine.h"

Engine::Engine() :
#ifdef _DEBUG
    logger(CS230::Logger::Severity::Debug, true, last_tick)
#else
    logger(CS230::Logger::Severity::Event, false, last_tick)
#endif
{ 
}

void Engine::Start(std::string window_title) {
    logger.LogEvent("Engine Started");
    window.Start(window_title);
    last_tick = std::chrono::system_clock::now();
    last_test = last_tick;
    unsigned int seed = static_cast<unsigned int>(time(NULL));
    std::srand(seed);
    logger.LogEvent("Random Seed: " + std::to_string(seed));
}

void Engine::Stop() {
    logger.LogEvent("Engine Stopped");
}

void Engine::Update() {

    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    double dt = std::chrono::duration<double>(now - last_tick).count();

    if (dt > 1.0 / TargetFPS)
    {
        logger.LogVerbose("Engine Update");
        last_tick = now;
        frame_count++;

        if (frame_count >= FPSTargetFrames)
        {
            double actual_time = std::chrono::duration<double>(now - last_test).count();
            double fps = frame_count / actual_time;
            logger.LogDebug("FPS: " + std::to_string(fps));
            frame_count = 0;
            last_test = now;
        }

        gamestatemanager.Update(dt);
        input.Update();
        window.Update();
    }
    //Update other services
}

bool Engine::HasGameEnded()
{
    if (gamestatemanager.HasGameEnded() || window.IsClosed())
    {
        return true;
    }
}

void Engine::AddFont(const std::filesystem::path& file_name) {
    fonts.push_back(file_name);
}