/*
Copyright (C) 2023 DigiPen Institute of Technology
Reproduction or distribution of this file or its contents without
prior written consent is prohibited
File Name:  Engine.h
Project:    CS230 Engine
Author:     Jonathan Holmes
Created:    March 8, 2023
*/

#pragma once

#include "Logger.h"
#include "GameStateManager.h"
#include "Input.h"
#include "TextureManager.h"
#include "IProgram.h"
#include "GLApp.h"
#include "Render.h"
#include "ShaderManager.h"
#include "AudioManager.h"
#include "FontManager.h"
#include "WindowState.h"
#include "IconManager.h"

#include <chrono>
#include <time.h>
#include <random>

extern std::random_device rd;

class Engine : public IProgram {
public:
    static Engine& Instance() {
        static Engine instance;
        return instance;
    }

    static ShaderManager& GetShaderManager() {
        return Instance().shadermanager;
    }
    static Logger& GetLogger() {
        return Instance().logger;
    }

    static GameStateManager& GetGameStateManager() {
        return Instance().gamestatemanager;
    }

    static Input& GetInput() {
        return Instance().input;
    }

    static TextureManager& GetTextureManager() {
        return Instance().texturemanager;
    }

    static Render& GetRender() {
        return Instance().render;
    }


    static AudioManager& GetAudioManager() {
        return Instance().audiomanager;
    }

    static FontManager& GetFontManager() {
        return Instance().fontmanager;
    }

    static IconManager& GetIconManager() {
        return Instance().iconmanager;
    }

    void Start(std::string window_title);
    void Stop();
    void Update();
    bool HasGameEnded();
    void HandleEvent(SDL_Window& sdl_window, const SDL_Event& event);
    void ImGuiDraw() override;
    void SetTmpPtr(void* tmp);
    void* GetTmpPtr();
    void UnloadTmpPtr();
    void SetSlowDownFactor(double slow_down) { slow_down_factor = slow_down; }
    void ResetSlowDownFactor() { slow_down_factor = 1; }
    double GetDt() { return dt; }
    static constexpr int window_height = 720;
    static constexpr int window_width = 1280;

    static constexpr double TargetFPS = 240.0;

private:
    Engine();

    std::chrono::system_clock::time_point last_tick = std::chrono::system_clock::now();
    std::chrono::system_clock::time_point last_test;

    int frame_count = 0;
    double slow_down_factor = 1;
    static constexpr int FPSDuration = 5;
    static constexpr int FPSTargetFrames = static_cast<int>(FPSDuration * TargetFPS);
    double dt;
    double FPS;
    double accumulator;
    void* tmp_ptr = nullptr;

    ShaderManager shadermanager;
    AudioManager audiomanager;
    Logger logger;
    GameStateManager gamestatemanager;
    Input input;
    TextureManager texturemanager;
    Render render;
    FontManager fontmanager;
    IconManager iconmanager;
};