#include "ScenarioComponent.h"

void ScenarioComponent::Load()
{
    event_manager.AddEvent(Event(
        [&]() { return save_manager.GetSaveData().day == 1; },
        [&]() {
            dialog->LoadDialogGroup("dialog-1", 1);
        }
    ));
}

void ScenarioComponent::Update(double dt)
{
    event_manager.Update();
}
