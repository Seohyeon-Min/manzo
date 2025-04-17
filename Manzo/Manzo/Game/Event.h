#pragma once
#include <string>
#include <functional>

class Event {
public:
    Event(std::string id,
        std::function<bool()> triggerFunc,
        std::function<void()> actionFunc);

    void Update();
    void Reset();

private:
    std::string            id;
    std::function<bool()>  trigger;
    std::function<void()>  action;
    bool                   triggered;
};