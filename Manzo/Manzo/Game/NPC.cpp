#include "NPC.h"

NPC::NPC(vec2 start_position) :
	Player(start_position)
{
	AddGOComponent(new Sprite("assets/images/ship.spt", this));
	walk_timer = new Timer(0.0);
	AddGOComponent(walk_timer);
}

void NPC::Update(double dt)
{
	GameObject::Update(dt);
}

bool NPC::Walk()
{
    if (!has_run) {
        walk_timer->Set(30.);
        has_run = true;
    }

    if (!walk_timer->IsFinished()) {
        SetPosition({ GetPosition().x + 2.f, GetPosition().y });
        return false; // °È´Â Áß
    }

    has_run = false;
    return true; // °È±â ³¡
}

void NPC::Draw(DrawLayer drawlayer)
{
	GameObject::Draw(drawlayer);
}
