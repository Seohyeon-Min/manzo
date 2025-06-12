/*
Copyright (C) 2023 DigiPen Institute of Technology
Reproduction or distribution of this file or its contents without
prior written consent is prohibited
File Name:  GameStateManager.h
Project:    CS230 Engine
Author:     Jonathan Holmes, Seohyeon Min
Created:    March 8, 2023
Updated:    03/14/2024
*/


#pragma once
#include "GameState.h"
#include <vector>
#include <thread>
#include <unordered_set>


class GameStateManager {
public:
    GameStateManager();

    void Update(double dt);
    void FixedUpdate(double dt);

    void AddGameState(GameState& gamestate);
    void SetNextGameState(int index);
    void ClearNextGameState();
    void ReloadState();
    void LoadPreviousGameState();
    bool HasGameEnded();

    bool IsNull() { return current_gamestate == nullptr; }
    std::string GetStateName() { return current_gamestate->GetName(); }
    template<typename T>
    T* GetGSComponent() { return current_gamestate->GetGSComponent<T>(); }

    void SetFromOption(bool flag) { option = flag; }
    bool FromOption() { return option; }
    std::string GetPreviouseStateName();

private:
    enum class Status {
        STARTING,
        LOADING,
        UPDATING,
        UNLOADING,
        STOPPING,
        EXIT
    };

    Status status;
    std::vector<GameState*> gamestates;
    GameState* current_gamestate;
    GameState* next_gamestate;

    int prev_index;
    bool option;
};
