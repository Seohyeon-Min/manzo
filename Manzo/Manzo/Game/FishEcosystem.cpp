
#include "FishEcosystem.h"
#include "AI.h"
#include "../Engine/GameObjectManager.h"

FishGenerator::FishGenerator()
{
	timer = new CS230::Timer(2.0);

	for (int i = 0; i < 250; i++)
	{
		BackgroundFish* bg_fish = new BackgroundFish();
		Engine::GetGameStateManager().GetGSComponent<CS230::GameObjectManager>()->Add(bg_fish);
		bg_fish->AddBackgroundFishes(bg_fish);
	}
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

		//generate object fishes
        if (newFish->type == Fish::FishType::Fish3)
        {
            int shape_index = rand() % formations.size();
            const auto& selectedFormation = formations[shape_index];

            for (const auto& offset : selectedFormation.offsets)
            {
                Fish* additionalFish = new Fish(newFish);

                float randomX = rand() % (int)(selectedFormation.randomOffsetMaxX - selectedFormation.randomOffsetMinX)
                    + selectedFormation.randomOffsetMinX;
                float randomY = rand() % (int)(selectedFormation.randomOffsetMaxY - selectedFormation.randomOffsetMinY)
                    + selectedFormation.randomOffsetMinY;

                vec2 randomOffset = { randomX, randomY };

                additionalFish->SetPosition(newFish->GetPosition() + offset + randomOffset);
                additionalFish->SetVelocity(newFish->GetVelocity());

                fishList.push_back(additionalFish);
                Engine::GetGameStateManager().GetGSComponent<CS230::GameObjectManager>()->Add(additionalFish);
            }
        }
    }
}

FishGenerator::~FishGenerator()
{
    delete timer;
    timer = nullptr;
    fishList.clear();
}
