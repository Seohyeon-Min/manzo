#pragma once
#include "../Engine/GameState.h"
#include "..\Engine\GameObjectManager.h"
#include "../Engine/GameState.h"
#include "../Engine/Sprite.h"
#include "..\Engine\GameObjectManager.h"
#include "../Engine/Timer.h"
#include "FishEcosystem.h"
#include "../Engine/Camera.h"
#include "Background.h"
#include "Skill.h"
#include "Fish.h"
#include "Shop.h"
#include "Module.h"
#include "DialogBox.h"

class Background;
class Ship;
class Reef;





class Mode3 : public GameState {
public:
    Mode3();
    void Load() override;
    void Update(double dt) override;
    void FixedUpdate(double dt) override;
    void Unload() override;
    void Draw() override;

    std::string GetName() override {
        return "Mode3";
    }

private:

    enum class TutorialPhase {
        Init,
        Dash,
        Done
    };

    TutorialPhase currentPhase = TutorialPhase::Init;


    Ship* ship_ptr;
    Beat* beat_system;
    Reef* reef_ptr;
    Cam* camera;
    Background* background;
    Dialog* dialog_ptr;
    double phaseTimer = 0.0;
    bool textDisplay = false;

    int dash_count = 0;
    int beat_count = 0;
    int success_count = 0;
    bool hasCheckedThisBeat = false;
    bool hasSucceededThisBeat = false;
    bool hasLoadedDialog = false;
};