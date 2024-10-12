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

    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    dt = std::chrono::duration<double>(now - last_tick).count();

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
            
            environment.mouseX = static_cast<int>(GetInput().GetMousePosition().x);
            environment.mouseY = static_cast<int>(GetInput().GetMousePosition().y);
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
        ImGui::LabelText("FPS", "%.1f", (FPS));
        ImGui::LabelText("Delta time", "%.4f seconds", static_cast<float>(dt));
        ImGui::LabelText("Mouse Positions", "Device(%4.0f,%4.0f)\nCamera(%4.0f,%4.0f)\nWorld (%4.0f,%4.0f)",
            static_cast<double>(environment.mouseX), static_cast<double>(environment.mouseY),                                                             // device space
            static_cast<double>(environment.mouseCamSpaceX), static_cast<double>(environment.mouseCamSpaceY),    // in Camera Space
            static_cast<double>(environment.mouseWorldSpaceX), static_cast<double>(environment.mouseWorldSpaceY) // in World Space
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
    }
    ImGui::End();
}

bool Engine::HasGameEnded() {
    if (gamestatemanager.HasGameEnded()) {
        return true;
    }
    return false;
}
