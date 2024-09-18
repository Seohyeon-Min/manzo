#include "Fish.h"
#include "../Engine/GameObjectManager.h"
#include "../Engine/Collision.h"

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

    if (GetPosition().x - GetGOComponent<CS230::Sprite>()->GetFrameSize().x/2 > Engine::GetWindow().GetSize().x ||
        GetPosition().y + GetGOComponent<CS230::Sprite>()->GetFrameSize().y/2 < 0 || 
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

/*
void Fish::ResolveCollision(GameObject* other_object)
{
    Math::rect fish_rect = GetGOComponent<CS230::RectCollision>()->WorldBoundary();
    Math::rect other_rect = other_object->GetGOComponent<CS230::RectCollision>()->WorldBoundary();

    switch (other_object->Type())
    {
    case GameObjectTypes::Reef:
        if (fish_rect.Top() >= other_rect.Bottom()) //¹°°í±â À§¿¡¶û reef ¹Ø¿¡¶û ´ê¾ÒÀ» ¶§
        {

        }
        else if (fish_rect.Bottom() <= other_rect.Top()) //¹°°í±â ¹Ù´ÚÀÌ¶û reef À§¶û ´ê¾ÒÀ» ¶§
        {

        }
        else if (fish_rect.Right() >= other_rect.Left()) //¹°°í±â ¿À¸¥ÂÊÀÌ¶û reef ¿ÞÂÊÀÌ¶û ´ê¾ÒÀ» ¶§
        {

        }
        else if (fish_rect.Left() <= other_rect.Right()) //¹°°í±â ¿ÞÂÊÀÌ¶û reef ¿À¸¥ÂÊÀÌ¶û ´ê¾ÒÀ» ¶§
        {

        }
        break;
    }
}

bool Fish::CanCollideWith(GameObjectTypes obj)
{
    return obj == GameObjectTypes::Reef;
}

*/