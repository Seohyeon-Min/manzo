#include "ScenarioComponent.h"

void ScenarioComponent::Load()
{
    event_manager.AddEvent(Event(
        [&]() { return save_manager.GetSaveData().day == 1; },
        [&]() {
            //RunDay1Logic();
            //// ���� ����
            //auto& sd = saveMgr.GetSaveData();
            //sd.day = 2;
            //saveMgr.UpdateSaveData(sd);
        }
    ));
}

void ScenarioComponent::Update(double dt)
{
    event_manager.Update();
}
