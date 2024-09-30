/*
Copyright (C) 2023 DigiPen Institute of Technology
Reproduction or distribution of this file or its contents without
prior written consent is prohibited
File Name:  main.cpp
Project:    CS230 Engine
Author:     Jonathan Holmes
Created:    March 8, 2023
*/

#define SDL_MAIN_HANDLED 
#include <iostream>

#include "Engine/Engine.h"
#include "Game/Splash.h"
#include "Game/Mode1.h"

util::owner<IProgram*> create_program(int viewport_width, int viewport_height)
{
    glCheck(glViewport(0, 0, viewport_width, viewport_height));
    return &Engine::Instance();
}

int main() {
    try {
        GLApp OpenGLAPPlication("Manzo");
        Engine& engine = Engine::Instance();

        Mode1 mode1;
        engine.GetGameStateManager().AddGameState(mode1);

        while (!OpenGLAPPlication.IsDone() && !engine.HasGameEnded()) {
            engine.Update(); // Update the engine here
            OpenGLAPPlication.Update();
        }

        engine.Stop();
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
        return -1;
    }
}