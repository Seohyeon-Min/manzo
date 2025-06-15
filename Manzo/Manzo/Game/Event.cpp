#include "Event.h"

#include <algorithm>                     // std::find
#include "../Engine/Engine.h"           // Engine::GetSaveDataManager()
#include "../Engine/SaveDataManager.h"  // SaveDataManager
#include "SaveData.h"                   // SaveData (eventsDone ¸â¹ö)

Event::Event(std::string id,
    std::function<bool()> triggerFunc,
    std::function<void()> actionFunc)
    : id(std::move(id))
    , trigger(std::move(triggerFunc))
    , action(std::move(actionFunc))
    , triggered(false)
{}

void Event::Update() {
    auto& saveData = Engine::GetSaveDataManager().GetSaveData();
    auto& done = saveData.eventsDone;

    if (!triggered
        && std::find(done.begin(), done.end(), id) == done.end()
        && trigger())
    {
        action();
        triggered = true;
        done.push_back(id);
        Engine::GetSaveDataManager().UpdateSaveData(saveData);
    }
}

void Event::Reset() {
    triggered = false;

    auto& saveData = Engine::GetSaveDataManager().GetSaveData();
    auto& done = saveData.eventsDone;

    auto it = std::find(done.begin(), done.end(), id);
    if (it != done.end()) {
        std::cout << "Erasing from eventsDone: " << *it << "\n";
        done.erase(it);
        Engine::GetSaveDataManager().UpdateSaveData(saveData);
    }
    else {
        std::cout << "[Reset] NOT FOUND in eventsDone: " << id << "\n";
    }
}

StepEvent::StepEvent(const std::string& id) : id(id) {}

void StepEvent::AddStep(const std::function<bool()>& trigger, const std::function<void()>& action) {
    steps.emplace_back("step_" + std::to_string(steps.size()), trigger, action);
    std::cout << "[DEBUG] StepEvent '" << id << "' added\n";
}

void StepEvent::Update() {
    if (IsFinished()) return;
    std::cout << "[DEBUG] StepEvent update called\n";
    Event& step = steps[current_step];
    if (!step.HasRun()) {
        step.Update();
    }
    if (step.HasRun()) {
        ++current_step;
        if (IsFinished()) {
            Engine::GetEventManager().MarkEventDone(id);
            std::cout << "[StepEvent] Finished: " << id << std::endl;
        }
    }
}

bool StepEvent::IsFinished() const {
    return current_step >= steps.size();
}

const std::string& StepEvent::GetID() const {
    return id;
}