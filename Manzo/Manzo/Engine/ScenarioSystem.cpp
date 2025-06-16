#include "ScenarioSystem.h"
#include "../Game/NPC.h"
#include "GameObjectManager.h"
#include "../Game/DialogBox.h"
#include "../Game/States.h"
#include "../Game/PopUp.h"
#include "Icon.h"

void ScenarioSystem::LoadMode2Scenarios()
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
        }
    );

    intro->AddStep(
        [npc]() { return !npc->IsWalking(); },
        [this, npc]() {
            auto dlg = Engine::GetDialogSystem().GetDialog();
            if (!dlg) {
                std::cout << "dialog is null!" << std::endl;
            }
            dlg->LoadDialogGroup("day-1_1");
        }
    );

    intro->AddStep(
        [this, npc]() {auto dlg = Engine::GetDialogSystem().GetDialog();
    return dlg && dlg->IsFinished(); },
        []() {
            Engine::GetInput().SetMouseInputOn(true);
            Engine::GetGameStateManager().ClearNextGameState();
            Engine::GetGameStateManager().SetNextGameState(static_cast<int>(States::Tutorial));
        }
    );



    auto do_tuto = std::make_shared<StepEvent>("do_tutorial");

    do_tuto->AddStep(
        [this]() {
            return (!Engine::GetEventManager().HasEventDone("do_tutorial")) &&
                triggered;
        },
        [this]() {
            Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->SetCollisionON(false);
            auto dlg = Engine::GetDialogSystem().GetDialog();
            if (!dlg) {
                std::cout << "dialog is null!" << std::endl;
            }
            dlg->LoadDialogGroup("do_tutorial");
        }
    );

    do_tuto->AddStep(
        [this, npc]() {auto dlg = Engine::GetDialogSystem().GetDialog();
    return dlg && dlg->IsFinished(); },
        []() {
            Engine::GetInput().SetMouseInputOn(true);
            Engine::GetGameStateManager().ClearNextGameState();
            Engine::GetGameStateManager().SetNextGameState(static_cast<int>(States::Tutorial));
        }
    );

    //Engine::GetEventManager().AddEvent(Event("after_tutorial_end",
    //    [this]() {
    //        return Engine::GetEventManager().HasEventDone("tutorial_end") &&
    //            (Engine::GetGameStateManager().GetStateName() == "Mode2");
    //    },
    //    [this]() {
    //        // dialog->LoadDialogGroup("after_tutorial_end"); /////////////////////////////////////////////////////////////////
    //        auto* quest = new PopUp({ -420,195 }, "assets/images/quest_popup.spt", true);
    //        Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->Add(quest);
    //        quest->SetPop(true);

    //        Engine::GetEventManager().ResetEvent("after_tutorial_end");
    //    }
    //));

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
    Engine::GetEventManager().AddStepEvent(do_tuto);
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

void ScenarioSystem::Mode2Update()
{
    bool clicked = Engine::GetIconManager().IsIconClicked("can_go_sea");
    if (clicked) triggered = true;
    //std::cout << triggered<<" upader\n";
}
