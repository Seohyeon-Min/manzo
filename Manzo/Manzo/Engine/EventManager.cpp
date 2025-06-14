#include "EventManager.h"
#include "../Engine/Engine.h"
#include <algorithm>

void EventManager::AddEvent(const Event& e) {
    one_shot_events.push_back(e);
}

void EventManager::AddStepEvent(std::shared_ptr<StepEvent> s) {
    step_events.push_back(std::move(s));
}
void EventManager::RemoveStepEvent(const std::string& id) {
    auto it = std::remove_if(step_events.begin(), step_events.end(),
        [&](const std::shared_ptr<StepEvent>& e) { return e && e->GetID() == id; });
    step_events.erase(it, step_events.end());
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

void EventManager::ResetEvent(const std::string& id) {
    for (auto& e : one_shot_events) {
        if (e.GetID() == id) {
            std::cout << "ResetEvent called: " << id << "\n";
            e.Reset();
            break;
        }
    }
}

bool EventManager::HasEventDone(const std::string& id) const {
    return std::find(done_events.begin(), done_events.end(), id) != done_events.end();
}

void EventManager::MarkEventDone(const std::string& id)
{
    std::cout << "[MarkEventDone] called for: " << id << "\n";

    if (std::find(done_events.begin(), done_events.end(), id) == done_events.end()) {
        done_events.push_back(id);
    }

    auto& saveData = Engine::GetSaveDataManager().GetSaveData();
    auto& done = saveData.eventsDone;

    if (std::find(done.begin(), done.end(), id) == done.end()) {
        done.push_back(id);
        std::cout << "[MarkEventDone] pushed to eventsDone: " << id << "\n";
        Engine::GetSaveDataManager().UpdateSaveData(saveData); // ÀúÀå
    }
}

void EventManager::LoadSavedEvents(const std::vector<std::string>& saved) {
    done_events = saved;
}
