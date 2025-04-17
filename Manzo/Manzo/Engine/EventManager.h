#pragma once
#include "../Game/Event.h"
#include <vector>

class EventManager {
public:
    void AddEvent(const Event& event) {
        events.push_back(event);
    }

    void Update() {
        for (auto& event : events) {
            event.Update();
        }
    }

    void ResetAll() {
        for (auto& event : events) {
            event.Reset();
        }
    }

private:
    std::vector<Event> events; 
};