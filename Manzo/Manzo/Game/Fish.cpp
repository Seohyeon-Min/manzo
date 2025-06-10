#include "Fish.h"
#include "Effect.h"
#include "ScreenWrap.h"

#include "../Engine/GameObjectManager.h"
#include "../Engine/Camera.h"
#include "../Engine/Engine.h"

#include <random>
#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include <cmath>
#include <algorithm>
#include <map>
#include "FishEcosystem.h"

std::map<int, int> fishCollection;

Fish::Fish(int index, Fish* parent) : GameObject({ 0, 0 }) {
    map = Engine::GetGameStateManager().GetGSComponent<MapManager>()->GetMap(0);
    
    if (parent == nullptr) {
        bool isLeft = true; // rand() % 2;

        start_position = map->Spawn();//map->MaskToWorld(4700,5900);//{ startX , startY };
        
        SetPosition(start_position);
        
        //SetVelocity({ 0,0 });
        
        SetVelocity(isLeft ? Engine::GetGameStateManager().GetGSComponent<FishGenerator>()->fishBook[index].velocity : -Engine::GetGameStateManager().GetGSComponent<FishGenerator>()->fishBook[index].velocity);
        SetFlipX(!isLeft);

        SetScale(Engine::GetGameStateManager().GetGSComponent<FishGenerator>()->fishBook[index].scale);
        type = Engine::GetGameStateManager().GetGSComponent<FishGenerator>()->fishBook[index].type;
    }
    else {
        index = 2;
        type = parent->type;
        parentFish = parent;
    }

    AddGOComponent(new Sprite(Engine::GetGameStateManager().GetGSComponent<FishGenerator>()->fishBook[index].filePath, this));
    Engine::GetGameStateManager().GetGSComponent<FishGenerator>()->fishCnt++;
}

bool Fish::CanCollideWith(GameObjectTypes other_object) {
    switch (other_object) {
    case GameObjectTypes::Ship:
        return true;
    default:
        return false;
    }
}

void Fish::ResolveCollision(GameObject* other_object) {
    switch (other_object->Type()) {
    case GameObjectTypes::Ship:
        Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->Add(new CaptureEffect(GetPosition()));
        if (!collided)
        {
            collided = true;
            Engine::GetGameStateManager().GetGSComponent<FishGenerator>()->SetFishCnt(Engine::GetGameStateManager().GetGSComponent<FishGenerator>()->GetFishCnt() - 1);
            Engine::GetGameStateManager().GetGSComponent<FishGenerator>()->SetMoney(Engine::GetGameStateManager().GetGSComponent<FishGenerator>()->money + Engine::GetGameStateManager().GetGSComponent<FishGenerator>()->moneys[type-1]);
            fishCollection[type - 1]++;
        }
        this->Destroy();
        break;
    }
}

void Fish::Update(double dt) {
    GameObject::Update(dt);

    SetFlipX(GetVelocity().x <= 0);

    if (parentFish != nullptr) {
        if (parentFish->Destroyed()) {
            parentFish = nullptr;
        }
        else {
            SetVelocity(parentFish->GetVelocity());
            SetRotation(parentFish->GetRotation());
            return;
        }
    }

    //temp destroy range
    if (GetPosition().x < 0.f || GetPosition().x > 5800.f)
    {
        this->Destroy();
    }

    if(!map->IsMaskTrue(GetPosition()))
    {
        int max_attempts = 100;
        float try_distance = 100.0f; 

        for (int i = 0; i < max_attempts; ++i) {
            float angle_deg = RandomFloat(0.0f, 360.0f);
            float angle_rad = glm::radians(angle_deg);
            vec2 dir = { cos(angle_rad), sin(angle_rad) };
            vec2 testPos = GetPosition() + dir * try_distance;

            if (map->IsMaskTrue(testPos)) {
                SetVelocity(dir * GetVelocity().Length());  
                break;
            }
        }

    }
    else
    {

        //std::cout << "In Black" << std::endl;
    }
}

void Fish::Draw() {
    GameObject::Draw(DrawLayer::DrawLast);
}
