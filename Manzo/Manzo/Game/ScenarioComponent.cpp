#include "../Engine/GameObjectManager.h"
#include "../Engine/GameState.h"
#include "ScenarioComponent.h"
#include "NPC.h"

void ScenarioComponent::Load()
{
    static bool has_initialized = false;
    if (has_initialized) return;

    Engine::GetEventManager().AddEvent(Event("day1",
        [&]() { return Engine::GetSaveDataManager().GetSaveData().day == 1; },
        [&]() {
            NPC* npc = new NPC();
            Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->Add(npc);
            npc->Walk();
            dialog->LoadDialogGroup("day-1");
        }
    ));
    Engine::GetEventManager().AddEvent(Event("day2",
        [&]() { return Engine::GetSaveDataManager().GetSaveData().day == 2; },
        [&]() {
            dialog->LoadDialogGroup("day-2");
        }
    ));

    has_initialized = true;
}

