#include "../Engine/GameObjectManager.h"
#include "../Engine/GameState.h"
#include "ScenarioComponent.h"
#include "NPC.h"
#include "Mouse.h"

void ScenarioComponent::Load()
{
    static bool has_initialized = false;
    if (has_initialized) return;

    auto intro = std::make_shared<StepEvent>("npc_intro");
    std::shared_ptr<NPC> npc = std::make_shared<NPC>();

    intro->AddStep(
        []() { return Engine::GetSaveDataManager().GetSaveData().day == 1; },
        [npc]() {
            Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->Add(npc.get());
            npc->Walk();
            Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->SetCollisionON(false);
            Engine::GetInput().SetMouseInputOn(false);
            //Engine::GetIconManager().SetCollisionON(false);
        }
    );

    intro->AddStep(
        [npc]() { return !npc->IsWalking(); },
        [this, npc]() {
            if (!dialog) {
                std::cout << "dialog is null!" << std::endl;
            }
            dialog->LoadDialogGroup("day-1_1");
            //Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->GetGOComponent<Mouse>()->SetMouseOn(true);
            Engine::GetInput().SetMouseInputOn(false);
            
        }
    );

    intro->AddStep(
        [this, npc]() { return dialog->GetDialogFinish(); },
        []() {
            Engine::GetInput().SetMouseInputOn(true);
        }
    );



    Engine::GetEventManager().AddStepEvent(intro);
    //intro.AddStep(
    //    []() { return dialog->IsFinished(); },
    //    []() {
    //        Engine::GetScenarioManager().Unlock();
    //    }
    //);

    Engine::GetEventManager().AddEvent(Event("day2",
        [&]() { return Engine::GetSaveDataManager().GetSaveData().day == 2; },
        [&]() {
            dialog->LoadDialogGroup("day-2");
        }
    ));

    has_initialized = true;
}

