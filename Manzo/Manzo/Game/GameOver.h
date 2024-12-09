#pragma once
#include "..\Engine\Engine.h"
#include "../Engine/GameState.h"
#include "../Engine/Sprite.h"
#include "../Engine/Timer.h"
#include "Background.h"
#include "Skill.h"
#include "States.h"
#include "Fish.h"

class GameOver : public GameState {
public:
    GameOver();
    void Load() override;
    void Update(double dt) override;
    void Unload() override;
    void Draw() override;
    std::string GetName() override {
        return "GameOver";
    }
private:
    Background* background;
    Cam* camera;
    double time;
};