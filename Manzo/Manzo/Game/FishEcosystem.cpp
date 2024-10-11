#include "FishEcosystem.h"
#include "../Engine/GameObjectManager.h"

FishGenerator::FishGenerator()
{
    timer = new CS230::Timer(2.0);
    fishList.clear();
}

void FishGenerator::GenerateFish(double dt)
{
    timer->Update(dt);

    if (timer->Remaining() == 0)
    {
        Fish* newFish = new Fish();
        fishList.push_back(newFish);
        Engine::GetGameStateManager().GetGSComponent<CS230::GameObjectManager>()->Add(newFish);

        timer->Reset();

        //Generate School of Fish if parent's type is 3
        if (newFish->type == Fish::FishType::Fish3)
        {
            for (int i = 0; i < 4; i++)
            {
                Fish* additionalFish = new Fish(newFish);
                vec2 offset = { (rand() % 40 - 1) * 5.0f, (rand() % 40 - 1) * 5.0f };
                additionalFish->SetPosition({ newFish->GetPosition().x + offset.x ,newFish->GetPosition().y + offset.y });  //near parents
                additionalFish->SetVelocity(newFish->GetVelocity());
                fishList.push_back(additionalFish);
                Engine::GetGameStateManager().GetGSComponent<CS230::GameObjectManager>()->Add(additionalFish);
            }
        }
    }
}

void FishGenerator::DeleteFish()
{
    for (Fish* fish : fishList)
    {
        delete fish;
    }

    fishList.clear();
}
