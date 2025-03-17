
#define DEBUG_NEW new (_NORMAL_BLOCK, __FILE__, __LINE__)
#include "../Engine/GameObjectManager.h"
#include "FishEcosystem.h"
#include "AI.h"

FishGenerator::FishGenerator()
{
	timer = DEBUG_NEW Timer(2.0);

	//leader fish
	for (int i = 0; i < 6; i++)
	{
		BackgroundFish* bg_fish = DEBUG_NEW BackgroundFish();
		Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->Add(bg_fish);
		//bg_fish->change_state(&bg_fish->state_leader);
		bg_fish->current_state = &bg_fish->state_leader;
		bg_fish->current_state->Enter(bg_fish);
	}

	//non leader fish
	for (int i = 0; i < 60; i++)
	{
		BackgroundFish* bg_fish = DEBUG_NEW BackgroundFish();
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

			Fish* DEBUG_NEWFish = DEBUG_NEW Fish();
			fishList.push_back(DEBUG_NEWFish);
			Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->Add(DEBUG_NEWFish);

			timer->Reset();

			//generate object fishes
			if (DEBUG_NEWFish->type == Fish::FishType::Fish3)
			{
				int shape_index = rand() % formations.size();
				const auto& selectedFormation = formations[shape_index];

				for (const auto& offset : selectedFormation.offsets)
				{
					Fish* additionalFish = DEBUG_NEW Fish(DEBUG_NEWFish);

					float randomX = rand() % (int)(selectedFormation.randomOffsetMaxX - selectedFormation.randomOffsetMinX)
						+ selectedFormation.randomOffsetMinX;
					float randomY = rand() % (int)(selectedFormation.randomOffsetMaxY - selectedFormation.randomOffsetMinY)
						+ selectedFormation.randomOffsetMinY;

					vec2 randomOffset = { randomX, randomY };

					additionalFish->SetPosition(DEBUG_NEWFish->GetPosition() + offset + randomOffset);
					additionalFish->SetVelocity(DEBUG_NEWFish->GetVelocity());

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
