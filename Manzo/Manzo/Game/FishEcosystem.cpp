
#include "FishEcosystem.h"
#include "../Engine/GameObjectManager.h"

FishGenerator::FishGenerator()
{
	timer = new CS230::Timer(2.0);
	bg_timer = new CS230::Timer(5.0);
}

void FishGenerator::GenerateFish(double dt)
{
	timer->Update(dt);
	bg_timer->Update(dt);

	if (timer->Remaining() == 0)
	{
		Fish* newFish = new Fish();
		fishList.push_back(newFish);
		Engine::GetGameStateManager().GetGSComponent<CS230::GameObjectManager>()->Add(newFish);

		timer->Reset();

		//generate object fishes
		if (newFish->type == Fish::FishType::Fish3)
		{
			for (int i = 0; i < 4; i++)
			{
				//linked list �������� �������� �����ؾ� ���࿡ parent�� ������� �� next�� parent�� ������ �� ������
				//tree������ �� ������ ���⵵?
				//additionalFish�� ������ �� snake h.w �����ؼ� linked list�� �ƴϸ� tree�� �����
				Fish* additionalFish = new Fish(newFish);

				int formation_index = i + 1;
				vec2 offset;
				switch (formation_index) {
				case 1: offset = { -30, -30 }; break;
				case 2: offset = { -30, 30 }; break;
				case 3: offset = { -60, -60 }; break;
				case 4: offset = { -60, 60 }; break;
				default: offset = { 0, 0 }; break;
				}

				additionalFish->SetPosition(newFish->GetPosition() + offset);
				additionalFish->SetVelocity(newFish->GetVelocity());

				fishList.push_back(additionalFish);
				Engine::GetGameStateManager().GetGSComponent<CS230::GameObjectManager>()->Add(additionalFish);
			}
		}
	}


	//generate background fish
	if (bg_timer->Remaining() == 0)
	{
		for (int i = 0; i < 4; i++)
		{
			BackgroundFish* bg_fish = new BackgroundFish();
			Engine::GetGameStateManager().GetGSComponent<CS230::GameObjectManager>()->Add(bg_fish);
			bg_fish->AddBackgroundFishes(bg_fish);
		}

		bg_timer->Reset();
	}
}

FishGenerator::~FishGenerator()
{
    delete timer;
    timer = nullptr;
    fishList.clear();
}
