
#include "../Engine/GameObjectManager.h"
#include "FishEcosystem.h"
#include "AI.h"

std::mt19937 dre_fishIndex(rd());

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
	for (int i = 0; i < 60; i++)
	{
		BackgroundFish* bg_fish = new BackgroundFish();
		Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->Add(bg_fish);
		//bg_fish->change_state(&bg_fish->state_nonleader);

		bg_fish->current_state = &bg_fish->state_nonleader;
		bg_fish->current_state->Enter(bg_fish);
	}
}

void FishGenerator::ReadFishCSV(const std::string& filename)
{
	std::ifstream file(filename);
	if (!file.is_open()) {
		std::cerr << "Failed to open file: " << filename << std::endl;
		return;
	}

	std::string line, cell;
	std::getline(file, line);

	while (std::getline(file, line)) {
		std::stringstream linestream(line);
		FishDex f;
		float scaleSize, velocitySize;

		std::getline(linestream, cell, ',');
		f.type = static_cast<FishType>(std::stoi(cell));

		std::getline(linestream, cell, ',');
		scaleSize = std::stof(cell);
		f.scale = { scaleSize, scaleSize };

		std::getline(linestream, cell, ',');
		velocitySize = std::stof(cell);
		f.velocity = { velocitySize, 0 };

		std::getline(linestream, cell, ',');
		f.filePath = cell;

		std::getline(linestream, cell, ',');
		f.possibility = std::stof(cell);
		weights.push_back(f.possibility);

		std::getline(linestream, cell, ',');
		f.money = std::stoi(cell);
		moneys.push_back(f.money);

		fishBook.push_back(f);
	}
	file.close();
}

void FishGenerator::GenerateFish(double dt)
{
	timer->Update(dt);

	if (timer->Remaining() == 0)
	{
		fishList.resize(fishCnt);
		fishList.shrink_to_fit();

		if (fishList.size() < 20) //limit of fish num
		{
			std::discrete_distribution<> fishIndex(weights.begin(), weights.end());

			Fish* newFish = new Fish(fishIndex(dre_fishIndex));
			fishList.push_back(newFish);
			Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->Add(newFish);

			timer->Reset();

			//generate object fishes
			if (newFish->type == FishType::Fish3)
			{
				int shape_index = rand() % formations.size();
				const auto& selectedFormation = formations[shape_index];

				for (const auto& offset : selectedFormation.offsets)
				{
					Fish* additionalFish = new Fish(newFish->type, newFish);

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
}

int FishGenerator::ReturnFishMoney(int index)
{
	return fishBook[index - 1].money;
}

FishGenerator::~FishGenerator()
{
	delete timer;
	timer = nullptr;
	fishList.clear();
	//backgroundFishList.clear();
}
