#pragma once
#include "../Engine/GameState.h"
#include "../Engine/GameObjectManager.h"
#include "../Engine/Timer.h"
#include "Fish.h"
#include "Reef.h"

constexpr double fish_timer = 2.0;

class Sea : public CS230::GameState {
public:
    Sea();
    void Load() override;
    void Update(double dt) override;
    void Unload() override;
    void Draw() override;

    std::string GetName() override {
        return "Sea";
    }

public:
    double counter = 0.0;
    double swimming_range = 15.0;
    CS230::Timer* timer;
    std::vector<Fish*> fishList;
    Fish* fish;
    Reef* reef;
};
