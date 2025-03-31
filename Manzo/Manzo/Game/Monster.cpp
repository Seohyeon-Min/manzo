#include "Ship.h"
#include "Monster.h"
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Monster::Monster(Ship* ship, vec2 pos): 
	GameObject(pos), ship_ptr(ship), init_pos(pos)
{
    current_state = &state_stanby;
    current_state->Enter(this);
	AddGOComponent(new Sprite("assets/images/monster.spt", this));
    beat = &Engine::GetBeat();
    dash_timer = new RealTimeTimer(dash_time);
    AddGOComponent(dash_timer);
    movement_range = { 900, {2200,-2000}  };
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

    SetRotation(angle);
    if (angle >= -1.5f && angle < 1.5f) {
        // 왼쪽: 좌우 반전
        SetScale({ scale, scale });
    }
    else {
        // 오른쪽: 기존 scale 값 그대로 사용
        SetScale({ scale, -scale });
    }
}

void Monster::Draw(DrawLayer drawlayer)
{
    GameObject::Draw(drawlayer);
#ifdef _DEBUG
    DrawSight();
#endif // _DEBUG
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

bool Monster::CanCollideWith(GameObjectTypes other_object)
{
    switch (other_object) {
    case GameObjectTypes::Ship:
        return true;
        break;
    }

    return false;
}

std::array<vec2, 4> Monster::GetCollisionBoxPoints()
{
    Math::rect box = GetCollisionBox();
    return {
        vec2{ box.Left(), box.Top() },
        vec2{ box.Right(), box.Top() },
        vec2{ box.Right(), box.Bottom() },
        vec2{ box.Left(), box.Bottom() }
    };
}

void Monster::ResolveCollision(GameObject* other_object)
{
    if (other_object->Type() == GameObjectTypes::Ship) {
        if (current_state == &state_go) {
            change_state(&state_stanby);
        }
        if (ship_ptr->IsShipMoving()) {
            std::cout << "yammy\n";
        }
    }
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
}

void Monster::Stanby::Update(GameObject* object, double dt)
{

}

void Monster::Stanby::CheckExit(GameObject* object)
{
    Monster* monster = static_cast<Monster*>(object);
    if (monster->IsPlayerInSight(monster->ship_ptr->GetPosition())) {
        monster->change_state(&monster->state_alert);
    }

}

void Monster::Alert::Enter(GameObject* object)
{
    Monster* monster = static_cast<Monster*>(object);
    monster->position = monster->GetPosition();
    monster->offset = monster->initial_offset;
}

void Monster::Alert::Update(GameObject* object, double dt)
{
    Monster* monster = static_cast<Monster*>(object);

    vec2 target_direction = (monster->ship_ptr->GetPosition() - monster->GetPosition()).Normalize();
    monster->angle = std::atan2(target_direction.y, target_direction.x);

    monster->offset *= -0.957f;
    //monster->SetPosition({ monster->GetPosition().x, monster->position.y + monster->offset });
}


void Monster::Alert::CheckExit(GameObject* object)
{
    Monster* monster = static_cast<Monster*>(object);
    if (monster->offset <= 0.5 && monster->offset >= -0.5 && monster->beat->GetBeat()) {
        monster->SetPosition(monster->position);
        monster->change_state(&monster->state_go);
    }
    if (!monster->IsPlayerInRange(monster->ship_ptr->GetPosition())) {
        std::cout << "Player out of range\n";
        monster->change_state(&monster->state_goback);
    }
    else if ((monster->GetPosition() - monster->ship_ptr->GetPosition()).Length() > monster->dist_from_ship) {
        std::cout << "Player too far\n";
        monster->change_state(&monster->state_goback);
    }
}

void Monster::Dash::Enter(GameObject* object)
{
    Monster* monster = static_cast<Monster*>(object);
    monster->direction = { monster->ship_ptr->GetPosition().x - (monster->GetPosition().x), monster->ship_ptr->GetPosition().y - (monster->GetPosition().y) };
    monster->direction = monster->direction.Normalize();
    monster->SetVelocity(monster->direction * monster->speed);
    monster->dash_timer->Start();
    alpha = init_alpha;
}

void Monster::Dash::Update(GameObject* object, double dt)
{
    Monster* monster = static_cast<Monster*>(object);

    vec2 target_direction = (monster->ship_ptr->GetPosition() - monster->GetPosition()).Normalize();

    //alpha -= (float)dt; std::cout << alpha << std::endl;
    if (alpha <= 0.02f) {
        alpha = 0.02f;
    }
    monster->direction = monster->direction * (1.0f - alpha) + target_direction * alpha;
    monster->direction = monster->direction.Normalize();

    monster->angle = std::atan2(target_direction.y, target_direction.x);

    monster->SetVelocity(monster->direction * monster->speed);
}


void Monster::Dash::CheckExit(GameObject* object)
{
    Monster* monster = static_cast<Monster*>(object);
    if (monster->dash_timer->IsFinished()) {
        if (monster->IsPlayerInRange(monster->ship_ptr->GetPosition())) {
            monster->dash_timer->Reset();
            monster->change_state(&monster->state_stanby);
        }
    }
}


// It needs a better AI
void Monster::Goback::Enter(GameObject* object)
{
    Monster* monster = static_cast<Monster*>(object);
    monster->direction = { monster->init_pos.x - (monster->GetPosition().x), monster->init_pos.y - (monster->GetPosition().y) };
    monster->direction = monster->direction.Normalize();
    monster->SetVelocity(monster->direction * monster->speed);
}

void Monster::Goback::Update(GameObject* object, double dt)
{

}

void Monster::Goback::CheckExit(GameObject* object)
{
    Monster* monster = static_cast<Monster*>(object);
    float threshold = 15.0f; // 이 거리 이내로 도착하면 멈춤

    if ((monster->GetPosition() - monster->init_pos).Length() < threshold) {
        monster->SetVelocity({}); // 속도 0으로 설정
        monster->SetPosition(monster->init_pos); // 정확한 위치로 스냅 (선택 사항)
        monster->change_state(&monster->state_stanby);
    }
}

