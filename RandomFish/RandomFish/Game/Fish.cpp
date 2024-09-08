#include "Fish.h"
#include "../Engine/GameObjectManager.h"
#include "Sea.h"

Fish::Fish(Fish* parent) : CS230::GameObject({ 0, 0 }) {
    AddGOComponent(new CS230::Sprite("Assets/Fish.spt", this));
    if (parent == nullptr) {
        SetVelocity(Math::vec2{
            ((double)(rand() % 5) + 1) * default_velocity * 2,
            ((double)(rand() % 5) + 0.1)* default_velocity
            });

        Math::ivec2 windowSize = Engine::GetWindow().GetSize();
        start_position = { -23 ,((double)rand() / RAND_MAX) * windowSize.y};
        SetPosition(start_position);

        size = rand() % 3;
    }
    SetScale(Math::vec2{ -default_scales[size], default_scales[size] });
}


void Fish::Update(double dt) {
    GameObject::Update(dt);

    //remove fishes if they disappear from screen
    if (GetPosition().x - GetGOComponent<CS230::Sprite>()->GetFrameSize().x > Engine::GetWindow().GetSize().x)
    {
        Destroy();
    }

    //to limit fish's moving range (y axis)
    if (GetPosition().y >= start_position.y + swimming_range || GetPosition().y <= start_position.y - swimming_range)
    {
        SetVelocity({ GetVelocity().x, -GetVelocity().y });
    }
}