
#include "../Engine/GameObjectManager.h"
#include "FishEcosystem.h"
#include "AI.h"

FishGenerator::FishGenerator()
{
	timer = new Timer(2.0);

	//leader fish
	for (int i = 0; i < 6; i++)
	{
		BackgroundFish* bg_fish = new BackgroundFish();
		Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->Add(bg_fish);
		//bg_fish->change_state(&bg_fish->state_leader);
		bg_fish->current_state = &bg_fish->state_leader;
		bg_fish->current_state->Enter(bg_fish);
	}

	//non leader fish
	for (int i = 0; i < 240; i++)
	{
		BackgroundFish* bg_fish = new BackgroundFish();
		Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->Add(bg_fish);
		//bg_fish->change_state(&bg_fish->state_nonleader);

		bg_fish->current_state = &bg_fish->state_nonleader;
		bg_fish->current_state->Enter(bg_fish);
	}
}

void FishGenerator::GenerateFish(double dt)
{
	timer->Update(dt);

	if (timer->Remaining() == 0)
	{
		fishList.resize(fishCnt);
		fishList.shrink_to_fit();

		if (fishList.size() < 15) //limit of fish num
		{

			Fish* newFish = new Fish();
			fishList.push_back(newFish);
			Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->Add(newFish);

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
					Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->Add(additionalFish);
				}
			}
		}
	}

	quadtree.clear();
	for (auto& fish : backgroundFishList) {
		quadtree.insert(fish);
	}
}

FishGenerator::~FishGenerator()
{
	delete timer;
	timer = nullptr;
	fishList.clear();
	backgroundFishList.clear();
}
