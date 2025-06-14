#pragma once

#include "../Game/Event.h"
#include <vector>
#include <string>
#include <memory>

class EventManager {
public:
    void AddEvent(const Event& e);
    void AddStepEvent(std::shared_ptr<StepEvent> s);
    void RemoveStepEvent(const std::string& id);
    void Update();
    void ResetAll();

    void ResetEvent(const std::string& id);

    bool HasEventDone(const std::string& id) const;
    void MarkEventDone(const std::string& id);
    void LoadSavedEvents(const std::vector<std::string>& saved); // 저장된 이벤트 불러오기

private:
    std::vector<Event> one_shot_events;
    std::vector<std::shared_ptr<StepEvent>> step_events;
    std::vector<std::string> done_events; // 저장된 이벤트 리스트
};
