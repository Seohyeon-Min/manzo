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

#include <SDL2/SDL.h>
#include <iostream>
#include <imgui.h>
#include <GL/glew.h>

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
    using namespace std::chrono;

    // 현재 시간 계산
    system_clock::time_point now = system_clock::now();
    dt = duration<double>(now - last_tick).count();
    last_tick = now;

    // 누적 시간 갱신
    accumulator += dt;
    const double fixed_delta_time = 1.0 / TargetFPS;

    // FixedUpdate 호출
    if (!gamestatemanager.IsNull()) {
        while (accumulator >= fixed_delta_time) {
            gamestatemanager.FixedUpdate(fixed_delta_time);
            accumulator -= fixed_delta_time;
        }
    }

    // VariableUpdate 호출
    logger.LogVerbose("Engine Update");
    input.Update();
    gamestatemanager.Update(dt);

    // FPS 계산 (선택적)
    frame_count++;
    if (frame_count >= FPSTargetFrames) {
        double actual_time = duration<double>(now - last_test).count();
        FPS = frame_count / actual_time;
        logger.LogDebug("FPS: " + std::to_string(FPS));
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
    case SDL_MOUSEMOTION:
        // make sure we only save mouse positions for the main window and not some ImGui window
        if (event.motion.windowID == SDL_GetWindowID(&sdl_window))
        {
            // get the mouse device positions from event.motion.x/event.motion.y
            // save to environment.mouseX/mouseY


        }
        break;
    }

}

//void Engine::compute_mouse_coordinates() noexcept
//{
//    // Use camera view BuildWindowDeviceToCamera to convert to camera space
//    mat3 device_mouse = mat3::build_translation({ (float)environment.mouseX,(float)environment.mouseY });
//
//    //mat3 cam_mouse = caminfo.camera_view.BuildWindowDeviceToCamera() * device_mouse;
//    //// save to environment.mouseCamSpaceX/environment.mouseCamSpaceY
//    //environment.mouseCamSpaceX = cam_mouse.column2.x;
//    //environment.mouseCamSpaceY = cam_mouse.column2.y;
//
//    //// Use camera BuildCameraToWorld to convert to world space
//    //mat3 world_mouse = device_mouse * caminfo.camera.BuildCameraToWorld();
//    //// save to environment.mouseWorldSpaceX/environment.mouseWorldSpaceY
//    //environment.mouseWorldSpaceX = world_mouse.column2.x;
//    //environment.mouseWorldSpaceY = world_mouse.column2.y;
//}

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
        //const auto cam_pos = caminfo.camera.Position;
        //ImGui::LabelText("Camera World Position", "(%.1f,%.1f)", static_cast<double>(cam_pos.x), static_cast<double>(cam_pos.y));
        //const auto cam_orientation = int(util::to_degrees(caminfo.camera.GetOrientation()) + 360) % 360;
        //ImGui::LabelText("Camera World Orientation", "%d degrees", cam_orientation);
        //const auto cam_size = caminfo.camera_view.CalcViewSizeWithZoom();
        //ImGui::LabelText("Camera View Size", "%.1f x %.1f", static_cast<double>(cam_size.width), static_cast<double>(cam_size.height));
        //auto zoom = caminfo.camera_view.GetZoom() * 100.0f;
        //if (ImGui::SliderFloat("Zoom", &zoom, 25.0f, 400.0f, "%.0f%%"))
        //{
        //    caminfo.camera_view.SetZoom(zoom / 100.0f);
        //}
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
