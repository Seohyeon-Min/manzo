#include "Fish.h"
#include "../Engine/GameObjectManager.h"
#include "../Engine/Collision.h"

Fish::Fish(Fish* parent) : CS230::GameObject({ 0, 0 }) {
    AddGOComponent(new CS230::Sprite("assets/images/Fish.spt", this));
    if (parent == nullptr) {
        SetVelocity(vec2{
            ((float)(rand() % 5) + 1) * (float)default_velocity * 2,
            ((float)(rand() % 5) + 0.1f)* (float)default_velocity
            });

        ivec2 windowSize = { Engine::window_width, Engine::window_height };
        start_position = { -23 ,((float)rand() / RAND_MAX) * (float)windowSize.y};
        SetPosition(start_position);

        size = rand() % 3;
    }
    SetScale(vec2{ (float) - default_scales[size],(float)default_scales[size]});
}

bool Fish::CanCollideWith(GameObjectTypes other_object)
{
    switch (other_object) {
    case GameObjectTypes::Reef:
        return true;
        break;
    }
    return false;
}

void Fish::ResolveCollision(GameObject* other_object)
{
    switch (other_object->Type()) {
    case GameObjectTypes::Reef:
        Destroy();
        break;
    }
}

void Fish::Update(double dt) {
    GameObject::Update(dt);

    if (GetPosition().x - GetGOComponent<CS230::Sprite>()->GetFrameSize().x/2 > Engine::window_width||
        GetPosition().y + GetGOComponent<CS230::Sprite>()->GetFrameSize().y/2 < 0 || 
        GetPosition().y - GetGOComponent<CS230::Sprite>()->GetFrameSize().y / 2 > Engine::window_height)
    {
        Destroy();
    }

    //to limit fish's moving range (y axis)
    if (GetPosition().y >= start_position.y + swimming_range || GetPosition().y <= start_position.y - swimming_range)
    {
        SetVelocity({ GetVelocity().x, -GetVelocity().y });
    }
}

void Fish::Draw() {
    CS230::GameObject::Draw();
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