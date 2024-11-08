#include "FishEcosystem.h"
#include "../Engine/GameObjectManager.h"

FishGenerator::FishGenerator()
{
    timer = new CS230::Timer(2.0);
    bg_timer = new CS230::Timer(10.0);
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
                //linked list 느낌으로 물고기들을 생성해야 만약에 parent가 사라지면 그 next를 parent로 설정할 수 있을듯
                //tree구조가 더 나을거 같기도?
                //additionalFish를 생성할 때 snake h.w 참고해서 linked list나 아니면 tree로 만들기
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
        // Create the leader fish with a random Y position
        BackgroundFish* leaderFish = new BackgroundFish();
        leaderFish->SetPosition({ -640 ,((float)rand() / RAND_MAX) * 2.0f * height - height });
        Engine::GetGameStateManager().GetGSComponent<CS230::GameObjectManager>()->Add(leaderFish);
        backgroundFishList.push_back(leaderFish);

        BackgroundFish* lastFollower = leaderFish;  // Initialize the last follower as the leader

        // Create the follower fishes positioned directly behind the leader
        for (int i = 1; i < 10; i++)
        {
            BackgroundFish* followerFish = new BackgroundFish();
            followerFish->SetLeader(lastFollower);  // Set the leader for the follower
            backgroundFishList.push_back(followerFish);
            Engine::GetGameStateManager().GetGSComponent<CS230::GameObjectManager>()->Add(followerFish);

            // Position the follower fish behind the last one in the chain
            vec2 leaderPos = lastFollower->GetPosition();
            followerFish->SetPosition({ leaderPos.x - 30, leaderPos.y });

            lastFollower = followerFish;  // Update the last follower
        }


        bg_timer->Reset();
    }
}

FishGenerator::~FishGenerator()
{
    fishList.clear();
    backgroundFishList.clear();
}
