#pragma once
#include "../Game/Event.h"
#include "logger.h"
#include <vector>

class EventManager {
public:
    void AddEvent(const Event& e) { one_shot_events.push_back(e); }
    void AddStepEvent(std::shared_ptr<StepEvent> s) {
        if (!s) {
            Engine::GetLogger().LogError("Tried to add null StepEvent!");
            return;
        }
        step_events.push_back(std::move(s));
    }

    void Update() {
        for (auto& e : one_shot_events)
            if (!e.HasRun()) e.Update();

        for (auto& s : step_events)
            s->Update();
    }

    void ResetAll() {
        for (auto& event : one_shot_events) {
            event.Reset();
        }
    }

private:
    std::vector<Event> one_shot_events;
    std::vector<std::shared_ptr<StepEvent>> step_events;
};