#pragma once
#include "../Engine/Component.h"
#include "../Engine/EventManager.h"
#include "../Engine/SaveDataManager.h"
#include "DialogBox.h"

class ScenarioComponent : public Component {
public:
    ScenarioComponent();
    void Load();
    void Unload();

private:
    Dialog* dialog;
};