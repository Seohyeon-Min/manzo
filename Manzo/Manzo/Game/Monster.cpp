#include "Ship.h"
#include "BeatSystem.h"
#include "Monster.h"

Monster::Monster(Ship* ship): 
	GameObject(init_pos), ship_ptr(ship)
{
    current_state = &state_stanby;
    current_state->Enter(this);
	AddGOComponent(new Sprite("assets/images/ship.spt", this));
    beat = Engine::GetGameStateManager().GetGSComponent<Beat>();
    dash_timer = new RealTimeTimer(dash_time);
    AddGOComponent(dash_timer);
    movement_range = { 700, {100,100} };
}

void Monster::Update(double dt)
{
	GameObject::Update(dt);

    float decrease_duration = (float)beat->GetFixedDuration() - 0.1f;
    float delta_radius = (max_scale - 1.0f) / decrease_duration;
    float delta_alpha = 1 / decrease_duration;

    if (beat->GetBeat()) {
        scale = 1.0f;
        wait = true;
    }
    if (wait && !beat->GetIsOnBeat()) {
        scale = max_scale;
        wait = false;
    }

    if (!wait && scale > 1.0f) {
        scale -= delta_radius * (float)dt;
    }

    scale = std::max(scale, 1.0f);

    SetScale({ scale,scale });
}

void Monster::Draw(DrawLayer drawlayer)
{
    GameObject::Draw(drawlayer);
    DrawSight();
}

bool Monster::IsPlayerInSight(const vec2& playerPos)
{
    vec2 mob_pos = {GetPosition().x,GetPosition().y - offset};
    float distanceSquared = (playerPos - mob_pos).LengthSquared();

    return distanceSquared <= (sight_radius * sight_radius);
}

bool Monster::IsPlayerInRange(const vec2& playerPos)
{
    vec2 mob_pos = { movement_range.pos.x, movement_range.pos.y - offset };
    float distanceSquared = (playerPos - mob_pos).LengthSquared();

    return distanceSquared <= (movement_range.radius * movement_range.radius);
}

void Monster::DrawSight()
{
    CircleDrawCall draw_call = {
        sight_radius,
        {GetPosition().x,GetPosition().y - offset},
    };

    draw_call.settings.do_blending = true;
    draw_call.sorting_layer = DrawLayer::DrawUI;
    Engine::GetRender().AddDrawCall(std::make_unique<CircleDrawCall>(draw_call));

    CircleDrawCall draw_call2 = {
    movement_range.radius,
    movement_range.pos,
    };

    draw_call2.settings.do_blending = true;
    draw_call2.sorting_layer = DrawLayer::DrawUI;
    Engine::GetRender().AddDrawCall(std::make_unique<CircleDrawCall>(draw_call2));
}

void Monster::Stanby::Enter(GameObject* object)
{
    Monster* monster = static_cast<Monster*>(object);
    monster->SetVelocity({});
    std::cout << "Stanby\n";
}

void Monster::Stanby::Update(GameObject* object, double dt)
{

}

void Monster::Stanby::CheckExit(GameObject* object)
{
    Monster* monster = static_cast<Monster*>(object);
    if (monster->IsPlayerInSight(monster->ship_ptr->GetPosition())) {
        monster->change_state(&monster->state_alert);
        std::cout << "Stanby to Alter\n";
    }
}

void Monster::Alert::Enter(GameObject* object)
{
    Monster* monster = static_cast<Monster*>(object);
    monster->position = monster->GetPosition();
    monster->offset = monster->initial_offset;
    std::cout << "Enter\n";
}

void Monster::Alert::Update(GameObject* object, double dt)
{
    Monster* monster = static_cast<Monster*>(object);
    monster->offset *= -0.977f;
    monster->SetPosition({ monster->GetPosition().x, monster->position.y + monster->offset });
}


void Monster::Alert::CheckExit(GameObject* object)
{
    Monster* monster = static_cast<Monster*>(object);
    if (!monster->IsPlayerInSight(monster->ship_ptr->GetPosition())) {
        monster->change_state(&monster->state_stanby);
        std::cout << "Eixt\n";
    }
    if (monster->offset <= 0.5 && monster->offset >= -0.5 && monster->beat->GetBeat()) {
        monster->SetPosition(monster->position);
        monster->change_state(&monster->state_go);
        std::cout << "Go!!!!!!!!!!\n";
    }
}

void Monster::Dash::Enter(GameObject* object)
{
    Monster* monster = static_cast<Monster*>(object);
    monster->direction = { monster->ship_ptr->GetPosition().x - (monster->GetPosition().x), monster->ship_ptr->GetPosition().y - (monster->GetPosition().y) };
    monster->direction = monster->direction.Normalize();
    monster->SetVelocity(monster->direction * monster->speed);
    std::cout << "Dash_STart\n";
    monster->dash_timer->Start();
}

void Monster::Dash::Update(GameObject* object, double dt)
{

}

void Monster::Dash::CheckExit(GameObject* object)
{
    Monster* monster = static_cast<Monster*>(object);
    if (monster->dash_timer->IsFinished()) {
        if (monster->IsPlayerInRange(monster->ship_ptr->GetPosition())) {
            monster->dash_timer->Reset();
            monster->change_state(&monster->state_stanby);
            std::cout << "Finish\n";
        }
    }
}
