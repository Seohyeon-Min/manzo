#pragma once
#include "../Engine/Component.h"
#include "../Engine/EventManager.h"
#include "../Engine/SaveDataManager.h"
#include "DialogBox.h"
#include <memory>

class ScenarioComponent : public Component {
public:
    ScenarioComponent(std::shared_ptr<Dialog> dia)
        : dialog(dia) {}
    void Load();
    void Unload();

private:
    std::weak_ptr<Dialog> dialog;
};