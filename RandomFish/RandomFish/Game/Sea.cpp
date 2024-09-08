#include "../Engine/Engine.h"
#include "States.h"
#include "Sea.h"
#include "Fish.h"

Sea::Sea() { 

    timer = new CS230::Timer(fish_timer);
}

void Sea::Load() {
    counter = 0;
    AddGSComponent(new CS230::GameObjectManager());
}

void Sea::Draw() {
    Engine::GetWindow().Clear(0xFFFFFFFF);
    DrawRectangleGradientV(0, 0, Engine::GetWindow().GetSize().x, Engine::GetWindow().GetSize().y, { 24,129,140,255 }, { 2,94,115,255 });

    GetGSComponent<CS230::GameObjectManager>()->DrawAll(Math::TransformationMatrix());
}

void Sea::Update([[maybe_unused]] double dt) {
    UpdateGSComponents(dt);
    GetGSComponent<CS230::GameObjectManager>()->UpdateAll(dt);
    timer->Update(dt);

    if (timer->timer == 0)
    {
        GetGSComponent<CS230::GameObjectManager>()->Add(new Fish());
        timer->Reset();
    }
}

void Sea::Unload() {
    GetGSComponent<CS230::GameObjectManager>()->Unload();
    ClearGSComponents();
}
