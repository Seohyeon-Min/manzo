/*
Copyright (C) 2023 DigiPen Institute of Technology
Reproduction or distribution of this file or its contents without
prior written consent is prohibited
File Name:  GameStateManager.cpp
Project:    CS230 Engine
Author:     Jonathan Holmes, Seohyeon Min
Created:    March 8, 2023
Updated:    03/14/2024
*/

#include "GameStateManager.h"
#include "Engine.h"
#include "GameObjectManager.h"

GameStateManager::GameStateManager() :
    current_gamestate(nullptr),
    next_gamestate(nullptr),
    status(Status::STARTING)
{ }

void GameStateManager::AddGameState(GameState& gamestate) {
    gamestates.push_back(&gamestate);
}

void GameStateManager::SetNextGameState(int index) {
    if (current_gamestate != nullptr) {
        for (int i = 0; i < gamestates.size(); ++i) {
            if (gamestates[i] == current_gamestate) {
                prev_index = i;
                break;
            }
        }
    }
    next_gamestate = gamestates[index];
}

void GameStateManager::ReloadState() {
    status = Status::UNLOADING;
}

void GameStateManager::LoadPreviousGameState()
{
    next_gamestate = gamestates[prev_index];
}

void GameStateManager::ClearNextGameState() {
    status = Status::UNLOADING;
    next_gamestate = nullptr;
}

bool GameStateManager::HasGameEnded() {
    return status == Status::EXIT;
}

void GameStateManager::Update(double dt) {
    static std::atomic<bool> isLoading{ false }; // 로딩 상태 플래그
    static std::thread loadingThread;

    switch (status) {
    case Status::STARTING:
        next_gamestate = gamestates[0];
        status = Status::LOADING;
        if (gamestates.size() < 1) {
            status = Status::STOPPING;
        }

        break;
    case Status::LOADING:
        current_gamestate = next_gamestate;
        Engine::GetLogger().LogEvent("Load " + current_gamestate->GetName());
        current_gamestate->Load();
        Engine::GetLogger().LogEvent("Load Complete");

        status = Status::UPDATING;
        break;
    case Status::UPDATING:
        if (current_gamestate != next_gamestate) {
            status = Status::UNLOADING;
        }
        else {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            Engine::GetLogger().LogVerbose("Update" + current_gamestate->GetName());
            current_gamestate->Update(dt);
            //if (current_gamestate->GetGSComponent<GameObjectManager>() != nullptr) {
            //    current_gamestate->GetGSComponent<GameObjectManager>()->CollisionTest();
            //}
            current_gamestate->Draw();
        }
        break;
    case Status::UNLOADING:
        Engine::GetLogger().LogEvent("Unload " + current_gamestate->GetName());
        current_gamestate->Unload();
        Engine::GetTextureManager().Unload();
        Engine::GetLogger().LogEvent("Unload Complete.");
        if (next_gamestate == nullptr) {
            status = Status::STOPPING;
        }
        else {
            status = Status::LOADING;
        }
        break;
    case Status::STOPPING:
        status = Status::EXIT;

        break;
    case Status::EXIT:
        break;
    }
}

void GameStateManager::FixedUpdate(double dt)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Engine::GetLogger().LogVerbose("FixedUpdate" + current_gamestate->GetName());
    current_gamestate->FixedUpdate(dt);
    if (current_gamestate->GetGSComponent<GameObjectManager>() != nullptr) {
        current_gamestate->GetGSComponent<GameObjectManager>()->CollisionTest();
    }

}
