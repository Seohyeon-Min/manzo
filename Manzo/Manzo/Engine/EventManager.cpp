#include "EventManager.h"
#include "../Engine/Engine.h"
#include <algorithm>

void EventManager::AddEvent(const Event& e) {
    one_shot_events.push_back(e);
}

void EventManager::AddStepEvent(std::shared_ptr<StepEvent> s) {
    step_events.push_back(std::move(s));
}

void EventManager::Update() {
    for (auto& e : one_shot_events)
        if (!e.HasRun()) e.Update();

    for (auto& s : step_events)
        s->Update();
}

void EventManager::ResetAll() {
    for (auto& event : one_shot_events)
        event.Reset();
}

bool EventManager::HasEventDone(const std::string& id) {
    const auto& done = Engine::GetSaveDataManager().GetSaveData().eventsDone;
    return std::find(done.begin(), done.end(), id) != done.end();
}
