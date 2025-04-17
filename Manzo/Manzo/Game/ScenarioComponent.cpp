#include "ScenarioComponent.h"

void ScenarioComponent::Load()
{
    event_manager.AddEvent(Event("day1",
        [&]() { return save_manager.GetSaveData().day == 1; },
        [&]() {
            dialog->LoadDialogGroup("day-1");
        }
    ));
    event_manager.AddEvent(Event("day2",
        [&]() { return save_manager.GetSaveData().day == 2; },
        [&]() {
            dialog->LoadDialogGroup("day-2");
        }
    ));
}

void ScenarioComponent::Update(double dt)
{
    event_manager.Update();
}
