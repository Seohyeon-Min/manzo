#pragma once
#include "../Engine/Component.h"
#include "../Engine/EventManager.h"
#include "../Engine/SaveDataManager.h"

class ScenarioComponent : public Component {
public:
    ScenarioComponent(EventManager& em, SaveDataManager& sdm)
        : event_manager(em), save_manager(sdm) {}
    void Load();
    void Update(double dt) override;
    void Unload();

private:
    EventManager& event_manager;
    SaveDataManager& save_manager;
};