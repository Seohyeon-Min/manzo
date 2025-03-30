#include "Box.h"
#include "Effect.h"
#include "ScreenWrap.h"

#include "../Engine/GameObjectManager.h"
#include "../Engine/AABB.h"
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


Box::Box(vec2 position) : GameObject(position) 
{
    AddGOComponent(new Sprite("assets/images/box.spt", this));
}

bool Box::CanCollideWith(GameObjectTypes other_object) {
    switch (other_object) {
    case GameObjectTypes::Ship:
    case GameObjectTypes::Net:
        return true;
    default:
        return false;
    }
}

void Box::ResolveCollision(GameObject* other_object) {
    switch (other_object->Type()) {
    case GameObjectTypes::Ship:
        Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->Add(new CaptureEffect(GetPosition()));
        
        this->Destroy();
        break;

    case GameObjectTypes::Net:
        Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->Add(new CaptureEffect(GetPosition()));
        
        this->Destroy();
        break;

    }
}

void Box::Update(double dt) {
    GameObject::Update(dt);

}

void Box::Draw() {
    GameObject::Draw(DrawLayer::DrawFirst);
}