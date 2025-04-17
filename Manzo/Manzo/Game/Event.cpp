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
}