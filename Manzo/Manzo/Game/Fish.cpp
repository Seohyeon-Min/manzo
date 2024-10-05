#include "Fish.h"
#include <random>

std::mt19937 dre;

Fish::Fish(Fish* parent) : CS230::GameObject({ 0, 0 }) {

    std::uniform_int_distribution<int> random_value(0, 99);
    std::uniform_int_distribution<int> size(0, 2);

    if (parent == nullptr) {
        SetVelocity(Math::vec2{
            ((double)(rand() % 5) + 1) * default_velocity * 2,
            ((double)(rand() % 5) + 0.1) * default_velocity
            });

        Math::ivec2 windowSize = Engine::GetWindow().GetSize();
        start_position = { -23 ,((double)rand() / RAND_MAX) * windowSize.y }; //outside of window
        SetPosition(start_position);

        if (random_value(dre) < 20)  //20%
        {
            type = FishType::Fish3;
        }
        else if (random_value(dre) < 60)  //40%
        {
            type = FishType::Fish1;
        }
        else  //40%
        {
            type = FishType::Fish2;
        }

    }
    else
    {
        type = parent->type;
        SetVelocity(parent->GetPosition());
        SetPosition(parent->GetPosition());
        SetRotation(parent->GetRotation());
    }

    SetScale(Math::vec2{ -default_scales[size(dre)], default_scales[size(dre)] });


    if (this->type == FishType::Fish2)
    {
        SetVelocity({ GetVelocity().x * 3.f, GetVelocity().y * 3.f });
    }

    switch (type)
    {
    case FishType::Fish1:
        AddGOComponent(new CS230::Sprite("Assets/Fish1.spt", this));
        break;

    case FishType::Fish2:
        AddGOComponent(new CS230::Sprite("Assets/Fish2.spt", this));
        break;

    case FishType::Fish3:
        AddGOComponent(new CS230::Sprite("Assets/Fish3.spt", this));
        break;
    }

}

bool Fish::CanCollideWith(GameObjectTypes other_object)
{
    switch (other_object) {
    case GameObjectTypes::Ship:
        return true;
        break;
    }
    return false;
}

void Fish::ResolveCollision(GameObject* other_object)
{
    switch (other_object->Type()) {
    case GameObjectTypes::Ship:
        this->Destroy();
        break;
    }
}

void Fish::Update(double dt) {
    GameObject::Update(dt);

    if (GetPosition().x - GetGOComponent<CS230::Sprite>()->GetFrameSize().x / 2 > Engine::GetWindow().GetSize().x ||
        GetPosition().y + GetGOComponent<CS230::Sprite>()->GetFrameSize().y / 2 < 0 ||
        GetPosition().y - GetGOComponent<CS230::Sprite>()->GetFrameSize().y / 2 > Engine::GetWindow().GetSize().y)
    {
        Destroy();
    }

    //to limit fish's moving range (y axis)
    if (GetPosition().y >= start_position.y + swimming_range || GetPosition().y <= start_position.y - swimming_range)
    {
        SetVelocity({ GetVelocity().x, -GetVelocity().y });
    }
}

void Fish::Draw(Math::TransformationMatrix camera_matrix) {
    CS230::GameObject::Draw(camera_matrix);
}
