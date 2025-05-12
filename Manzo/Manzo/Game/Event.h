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
    bool HasRun() { return triggered; }
    const std::string& GetID() const { return id; }

private:
    std::string            id;
    std::function<bool()>  trigger;
    std::function<void()>  action;
    bool                   triggered;
};

class StepEvent {
public:
    StepEvent(const std::string& id);

    void AddStep(const std::function<bool()>& trigger, const std::function<void()>& action);
    void Update();
    bool IsFinished() const;
    const std::string& GetID() const;

private:
    std::string id;
    std::vector<Event> steps;
    size_t current_step = 0;
};