#pragma once

#include "../Game/Event.h"
#include <vector>
#include <string>
#include <memory>

class EventManager {
public:
    void AddEvent(const Event& e);
    void AddStepEvent(std::shared_ptr<StepEvent> s);
    void Update();
    void ResetAll();
    bool HasEventDone(const std::string& id);

private:
    std::vector<Event> one_shot_events;
    std::vector<std::shared_ptr<StepEvent>> step_events;
};
