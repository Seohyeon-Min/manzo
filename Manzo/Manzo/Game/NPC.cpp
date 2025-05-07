#include "NPC.h"


NPC::NPC(vec2 start_position) :
	GameObject({ -650,-115 })
{
	AddGOComponent(new Sprite("assets/images/character_sprite/Merchant.spt", this));
	walk_timer = new RealTimeTimer(0.0);
	AddGOComponent(walk_timer);
    SetScale({ 2.0f, 2.0f });
}

void NPC::Update(double dt)
{
    GameObject::Update(dt);
}

void NPC::FixedUpdate(double fixed_dt)
{
    GameObject::FixedUpdate(fixed_dt);

    if (is_walking) {
        if (walk_timer->IsFinished()) {
            SetVelocity({});
            is_walking = false;
            std::cout << "Im Stop\n";
        }
        else {
            SetVelocity({ walking_speed, 0 });
            std::cout << "Im Moving\n";
            std::cout << GetPosition().x << "\n";
        }
    }
}


void NPC::Walk()
{
    if (!is_walking) {
        is_walking = true;
        walk_timer->Set(2.);
        walk_timer->Start();
    }
}
void NPC::Draw(DrawLayer drawlayer)
{
	GameObject::Draw(drawlayer);
}
