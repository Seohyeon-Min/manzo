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
#include "ShowCollision.h"
#include "../Game/BeatSystem.h"
#include "../Game/AI.h"

#include <SDL2/SDL.h>
#include <iostream>
#include <imgui.h>
#include <GL/glew.h>
#include "GameObject.h"

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
    last_test = last_tick;
}

void Engine::Stop() {
    logger.LogEvent("Engine Stopped");
}

void Engine::Update() {
    using namespace std::chrono;

    system_clock::time_point now = system_clock::now();
    dt = duration<double>(now - last_tick).count();
    dt /= slow_down_factor;
    last_tick = now;

    accumulator += dt;
    double fixed_delta_time = 1.0 / TargetFPS;
    fixed_delta_time /= slow_down_factor;

    if (!gamestatemanager.IsNull()) {
        while (accumulator >= fixed_delta_time) {
            gamestatemanager.FixedUpdate(fixed_delta_time);
            accumulator -= fixed_delta_time;
        }
    }

    gamestatemanager.Update(dt);
    input.Update();

    frame_count++;
    if (frame_count >= FPSTargetFrames) {
        double actual_time = duration<double>(now - last_test).count();
        FPS = frame_count / actual_time;
        logger.LogDebug("FPS: " + std::to_string(FPS));
        frame_count = 0;
        last_test = now;
    }
}


void Engine::HandleEvent(SDL_Window& sdl_window, const SDL_Event& event) {
    switch (event.type)
    {
    case SDL_QUIT << '\n':
        break;
    case SDL_MOUSEMOTION:
        if (event.motion.windowID == SDL_GetWindowID(&sdl_window))
        {
            // get the mouse device positions from event.motion.x/event.motion.y
            // save to environment.mouseX/mouseY


        }
        break;
    }
}

void Engine::ImGuiDraw()
{
    ImGui::Begin("Program Info");
    {
        float mouseX = GetInput().GetMousePosition().x;
        float mouseY = GetInput().GetMousePosition().y;
        float camX = GetInput().GetMousePos().mouseCamSpaceX;
        float camY = GetInput().GetMousePos().mouseCamSpaceY;
        float WorldX = GetInput().GetMousePos().mouseWorldSpaceX;
        float WorldY = GetInput().GetMousePos().mouseWorldSpaceY;

        ImGui::LabelText("FPS", "%.1f", (FPS));
        ImGui::LabelText("Delta time", "%.4f seconds", static_cast<float>(dt));
        ImGui::LabelText("Mouse Positions", "Device(%4.0f,%4.0f)\nCamera(%4.0f,%4.0f)\nWorld (%4.0f,%4.0f)",
            static_cast<double>(mouseX), static_cast<double>(mouseY),                                                             // device space
            static_cast<double>(camX), static_cast<double>(camY),    // in Camera Space
            static_cast<double>(WorldX), static_cast<double>(WorldY) // in World Space
        );
        
        if (!GetGameStateManager().IsNull())
        {

            ImGui::SliderFloat("alignment", &BackgroundFish::a, 0.0f, 5.0f, "%.4f");
            ImGui::SliderFloat("cohesion", &BackgroundFish::b, 0.0f, 5.0f, "%.4f");
            ImGui::SliderFloat("separate", &BackgroundFish::c, 0.0f, 5.0f, "%.4f");
            ImGui::SliderFloat("wanderForce", &BackgroundFish::d, 0.0f, 5.0f, "%.4f");
        }

        if (!GetGameStateManager().IsNull()
            && GetGameStateManager().GetGSComponent<Beat>() != nullptr) {
            ImGui::LabelText("Is on beat?", "%s", GetGameStateManager().GetGSComponent<Beat>()->GetIsOnBeat() ? "true" : "false");
            //ImGui::LabelText("Collision", "%s", GetGameStateManager().GetGSComponent<CS230::ShowCollision>()->Enabled() ? "true" : "false");
        }
    }
    ImGui::End();
}

bool Engine::HasGameEnded() {
    if (gamestatemanager.HasGameEnded()) {

    }
    return false;
}

void Engine::SetTmpPtr(void* ptr)
{
    tmp_ptr = ptr;
}
void* Engine::GetTmpPtr()
{
    return tmp_ptr;
}

void Engine::UnloadTmpPtr()
{
    delete tmp_ptr;
    tmp_ptr = nullptr;
}