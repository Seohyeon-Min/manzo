#include "Ship.h"
#include "Monster.h"
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Monster::Monster(Ship* ship, vec2 pos): 
	GameObject(pos), ship_ptr(ship)
{
    current_state = &state_stanby;
    current_state->Enter(this);
	AddGOComponent(new Sprite("assets/images/monster.spt", this));
    beat = &Engine::GetBeat();
    dash.timer = new RealTimeTimer(dash.dash_time);
    AddGOComponent(dash.timer);
    movement.init_pos = pos;
    movement.range = { 900, {2200,-2000}  };
}

void Monster::Update(double dt)
{
	GameObject::Update(dt);



    float decrease_duration = (float)beat->GetFixedDuration() - 0.1f;
    float delta_radius = (transform.max_scale - 1.0f) / decrease_duration;
    float delta_alpha = 1 / decrease_duration;

    if (beat->GetBeat()) {
        transform.scale = 1.0f;
        wait = true;
    }
    if (wait && !beat->GetIsOnBeat()) {
        transform.scale = transform.max_scale;
        wait = false;
    }

    if (!wait && transform.scale > 1.0f) {
        transform.scale -= delta_radius * (float)dt;
    }

    transform.scale = std::max(transform.scale, 1.0f);

    SetRotation(transform.angle);
    if (transform.angle >= -1.5f && transform.angle < 1.5f) {
        // 왼쪽: 좌우 반전
        SetScale({ transform.scale, transform.scale });
    }
    else {
        // 오른쪽: 기존 scale 값 그대로 사용
        SetScale({ transform.scale, -transform.scale });
    }
}

void Monster::SetUni(const GLShader* shader) {
    float time = 0.f;
    if (Engine::GetAudioManager().IsAnyMusicPlaying()) {
        time = Engine::GetAudioManager().GetCurrentPlayingMusicTime();
    }

    shader->SendUniform("uTime", time);
    shader->SendUniform("uX", 0);
    shader->SendUniform("uY", 1);
    shader->SendUniform("uWaveNum", 4);

    shader->SendUniform("waveStrength", 0.034f);
    shader->SendUniform("frequency", 27.0f);
    shader->SendUniform("speed", 4.4f);
}

void Monster::Draw(DrawLayer drawlayer)
{
    DrawCall draw_call = {
    GetGOComponent<Sprite>()->GetTexture(),					// Texture to draw
    &GetMatrix(),											// Transformation matrix
    Engine::GetShaderManager().GetShader("wave")
    };

    draw_call.settings.do_blending = true;
    draw_call.SetUniforms = [this](const GLShader* shader) { SetUni(shader); };

    GameObject::Draw(draw_call);
#ifdef _DEBUG
    DrawSight();
#endif // _DEBUG
}

bool Monster::IsPlayerInSight(const vec2& playerPos)
{
    vec2 mob_pos = {GetPosition().x,GetPosition().y - dash.offset};
    float distanceSquared = (playerPos - mob_pos).LengthSquared();

    return distanceSquared <= (vision.radius * vision.radius);
}

bool Monster::IsPlayerInRange(const vec2& playerPos)
{
    vec2 mob_pos = { movement.range.pos.x, movement.range.pos.y - dash.offset };
    float distanceSquared = (playerPos - mob_pos).LengthSquared();

    return distanceSquared <= (movement.range.radius * movement.range.radius);
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
            //std::cout << "yammy\n";
        }
    }
}


void Monster::DrawSight()
{
    CircleDrawCall draw_call = {
        vision.radius,
        {GetPosition().x,GetPosition().y - dash.offset},
    };

    draw_call.settings.do_blending = true;
    draw_call.sorting_layer = DrawLayer::DrawUI;
    Engine::GetRender().AddDrawCall(std::make_unique<CircleDrawCall>(draw_call));

    CircleDrawCall draw_call2 = {
    movement.range.radius,
    movement.range.pos,
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
    monster->movement.position = monster->GetPosition();
    monster->dash.offset = monster->dash.initial_offset;
}

void Monster::Alert::Update(GameObject* object, double dt)
{
    Monster* monster = static_cast<Monster*>(object);

    vec2 target_direction = (monster->ship_ptr->GetPosition() - monster->GetPosition()).Normalize();
    monster->transform.angle = std::atan2(target_direction.y, target_direction.x);

    monster->dash.offset *= -0.957f;
    //monster->SetPosition({ monster->GetPosition().x, monster->position.y + monster->offset });
}


void Monster::Alert::CheckExit(GameObject* object)
{
    Monster* monster = static_cast<Monster*>(object);
    if (monster->dash.offset <= 0.5 && monster->dash.offset >= -0.5 && monster->beat->GetBeat()) {
        monster->SetPosition(monster->movement.position);
        monster->change_state(&monster->state_go);
    }
    if (!monster->IsPlayerInRange(monster->ship_ptr->GetPosition())) {
        //std::cout << "Player out of range\n";
        monster->change_state(&monster->state_goback);
    }
    else if ((monster->GetPosition() - monster->ship_ptr->GetPosition()).Length() > monster->vision.dist_from_ship) {
        //std::cout << "Player too far\n";
        monster->change_state(&monster->state_goback);
    }
}

void Monster::Dash::Enter(GameObject* object)
{
    Monster* monster = static_cast<Monster*>(object);
    monster->movement.direction = { monster->ship_ptr->GetPosition().x - (monster->GetPosition().x), monster->ship_ptr->GetPosition().y - (monster->GetPosition().y) };
    monster->movement.direction = monster->movement.direction.Normalize();
    monster->SetVelocity(monster->movement.direction * monster->movement.speed);
    monster->dash.timer->Start();
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
    monster->movement.direction = monster->movement.direction * (1.0f - alpha) + target_direction * alpha;
    monster->movement.direction = monster->movement.direction.Normalize();

    monster->transform.angle = std::atan2(target_direction.y, target_direction.x);

    monster->SetVelocity(monster->movement.direction * monster->movement.speed);
}


void Monster::Dash::CheckExit(GameObject* object)
{
    Monster* monster = static_cast<Monster*>(object);
    if (monster->dash.timer->IsFinished()) {
        if (monster->IsPlayerInRange(monster->ship_ptr->GetPosition())) {
            monster->dash.timer->Reset();
            monster->change_state(&monster->state_stanby);
        }
    }
}


// It needs a better AI
void Monster::Goback::Enter(GameObject* object)
{
    Monster* monster = static_cast<Monster*>(object);
    monster->movement.direction = { monster->movement.init_pos.x - (monster->GetPosition().x), monster->movement.init_pos.y - (monster->GetPosition().y) };
    monster->movement.direction = monster->movement.direction.Normalize();
    monster->SetVelocity(monster->movement.direction * monster->movement.speed);
}

void Monster::Goback::Update(GameObject* object, double dt)
{

}

void Monster::Goback::CheckExit(GameObject* object)
{
    Monster* monster = static_cast<Monster*>(object);
    float threshold = 15.0f; // 이 거리 이내로 도착하면 멈춤

    if ((monster->GetPosition() - monster->movement.init_pos).Length() < threshold) {
        monster->SetVelocity({}); // 속도 0으로 설정
        monster->SetPosition(monster->movement.init_pos); // 정확한 위치로 스냅 (선택 사항)
        monster->change_state(&monster->state_stanby);
    }
}

