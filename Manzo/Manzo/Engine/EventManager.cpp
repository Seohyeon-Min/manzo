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
    for (auto& e : one_shot_events)
        e.Reset();
}

bool EventManager::HasEventDone(const std::string& id) const {
    return std::find(done_events.begin(), done_events.end(), id) != done_events.end();
}

void EventManager::MarkEventDone(const std::string& id) {
    if (!HasEventDone(id)) {
        done_events.push_back(id);
    }
}

void EventManager::LoadSavedEvents(const std::vector<std::string>& saved) {
    done_events = saved;
}
