#include "../Engine/GameObjectManager.h"
#include "../Engine/GameState.h"
#include "ScenarioComponent.h"
#include "NPC.h"
#include "Mouse.h"
#include "PopUp.h"
#include "States.h"

void ScenarioComponent::Load()
{

    Engine::GetEventManager().LoadSavedEvents(Engine::GetSaveDataManager().GetSaveData().eventsDone);

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
            //Engine::GetInput().SetMouseInputOn(false);
            
        }
    );

    intro->AddStep(
        [this, npc]() { return dialog->IsFinished(); },
        []() {
            Engine::GetInput().SetMouseInputOn(true);
            Engine::GetGameStateManager().ClearNextGameState();
            Engine::GetGameStateManager().SetNextGameState(static_cast<int>(States::Tutorial));
            
            //auto* quest = new PopUp({ -420,195 }, "assets/images/quest_popup.spt", true);
            //Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->Add(quest);
            //quest->SetPop(true);
        }
    );


    auto tuto_end = std::make_shared<StepEvent>("after_tutorial_end");

    tuto_end->AddStep(
        []() {
            return Engine::GetEventManager().HasEventDone("tutorial_end") &&
                Engine::GetGameStateManager().GetStateName() == "Mode2";
        },
        []() {
            Engine::GetEventManager().MarkEventDone("after_tutorial_ready");
            std::cout << "[StepEvent] Marked: after_tutorial_ready" << std::endl;
        }
    );



    tuto_end->AddStep(
        [this, npc]() { return dialog->IsFinished(); },
        []() {
            Engine::GetInput().SetMouseInputOn(true);
            Engine::GetGameStateManager().ClearNextGameState();
            Engine::GetGameStateManager().SetNextGameState(static_cast<int>(States::Tutorial));
            //auto* quest = new PopUp({ -420,195 }, "assets/images/quest_popup.spt", true);
            //Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->Add(quest);
            //quest->SetPop(true);
        }
    );

    Engine::GetEventManager().AddEvent(Event("after_tutorial_end",
        [this]() {
            return Engine::GetEventManager().HasEventDone("tutorial_end") &&
                (Engine::GetGameStateManager().GetStateName() == "Mode2");
        },
        [this]() {
            auto* quest = new PopUp({ -420,195 }, "assets/images/quest_popup.spt", true);
            Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->Add(quest);
            quest->SetPop(true);
            dialog_aftertuto = new Dialog({ 0,0 });
            if (Engine::GetEventManager().HasEventDone("after_tutorial_end")) {
                Engine::GetEventManager().ResetEvent("after_tutorial_end");
            }
            if (dialog_aftertuto != nullptr) {
                dialog_aftertuto->LoadDialogGroup("after_tutorial_end");
            }
            else {
                std::cout << "[Error] dialog is nullptr!" << std::endl;
            }

        }
    ));

    tuto_end->AddStep(
        []() {
            return Engine::GetEventManager().HasEventDone("after_tutorial_end");
        },
        [this]() {
            
        }
    );

    Engine::GetEventManager().AddEvent(Event("catch_15_fish",
        []() {
            const auto& fish = Engine::GetSaveDataManager().GetSaveData().fishCollection;
            int total = 0;
            for (const auto& [id, count] : fish) {
                total += count;
            }
            return total >= 15;
        },
        []() {
            std::cout << "물고기 15마리 이벤트 완료!" << std::endl;
            Engine::GetEventManager().MarkEventDone("catch_15_fish");

            auto* popup = new PopUp({ -420,195 }, "assets/images/catch_done_popup.spt", true);
            auto* manager = Engine::GetGameStateManager().GetGSComponent<GameObjectManager>();
            if (manager) {
                manager->Add(popup);
                popup->SetPop(true);
            }
            else {
                std::cout << "[catch_15_fish] GameObjectManager is null!" << std::endl;
            }
        }
    ));


    Engine::GetEventManager().AddStepEvent(intro);
    Engine::GetEventManager().AddStepEvent(tuto_end);
    //intro.AddStep(
    //    []() { return dialog->IsFinished(); },
    //    []() {
    //        Engine::GetScenarioManager().Unlock();
    //    }
    //);

    //Engine::GetEventManager().AddEvent(Event("day2",
    //    [&]() { return Engine::GetSaveDataManager().GetSaveData().day == 2; },
    //    [&]() {
    //        dialog->LoadDialogGroup("day-1_2");
    //    }
    //));

    has_initialized = true;
}

