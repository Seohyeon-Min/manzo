/*
Copyright (C) 2023 DigiPen Institute of Technology
Reproduction or distribution of this file or its contents without
prior written consent is prohibited
File Name:  Loading.h
Project:    Manzo
Author:     Won Kim
Created:    May 7, 2025
*/

#pragma once
#include "../Engine/GameState.h"


class Loading : public GameState {
public:
    Loading();

    void Load() override;
    void Update(double dt) override;
    void FixedUpdate(double dt) override;
    void Unload() override;
    void Draw() override;

    std::string GetName() override {
        return "Loading Screen";
    }
private:

};