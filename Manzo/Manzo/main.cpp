/*
Copyright (C) 2023 DigiPen Institute of Technology
Reproduction or distribution of this file or its contents without
prior written consent is prohibited
File Name:  main.cpp
Project:    CS230 Engine
Author:     Jonathan Holmes
Created:    March 8, 2023
*/

#include <iostream>

#include "Engine/Engine.h"
#include "Game/Splash.h"
#include "Game/Mode1.h"
#include "GLApp.h"

int main() {
    try {
        GLApp OpenGLApplication("Manzo");



        return 0;

        ////////////////////////////////////////////////////////


        Engine& engine = Engine::Instance();
        engine.Start("Manzo");

        Splash splash;
        engine.GetGameStateManager().AddGameState(splash);
        Mode1 mode1;
        engine.GetGameStateManager().AddGameState(mode1);

        while (engine.HasGameEnded() == false && !OpenGLApplication.IsDone()) {
            engine.Update();
        }

        engine.Stop();

        return 0;
    }
    catch (std::exception& e) {
        std::cerr << e.what() << "\n";
        return -1;
    }
}
