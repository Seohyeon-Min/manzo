#include "Ship.h"
#include "BeatSystem.h"
#include "../Engine/Camera.h"
#include "../Engine/Input.h"
#include <iostream>
#include "../Engine/MapManager.h"

Ship::Ship(vec2 start_position) :
    GameObject(start_position), moving(false), set_dest(false), ready_to_move(false), move(false)
{
    AddGOComponent(new CS230::Sprite("assets/images/ship.spt", this));
    beat = Engine::GetGameStateManager().GetGSComponent<Beat>();
    skill = Engine::GetGameStateManager().GetGSComponent<Skillsys>();
    fuel = Maxfuel;
    FuelFlag = false;
    SetVelocity({ 0,0 });
}

void Ship::Update(double dt)
{
    //std::cout << (!set_dest && beat->GetIsOnBeat() && !move) << std::endl;
    GameObject::Update(dt);

    if (Engine::GetGameStateManager().GetStateName() == "Mode1") {
        SetDest();

        if (move) {
            Move(dt);
            
        }
        else {
            if (hit_with) {
                vec2 velocity = GetVelocity();
                float velocity_magnitude = sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
                if (velocity_magnitude < 20.f) SetVelocity(direction * skidding_speed);
                else SetVelocity(GetVelocity() * deceleration);
                if (!beat->GetIsOnBeat()) {
                    SetVelocity(direction * skidding_speed);
                    hit_with = false;
                    if (!clickable) { // wait for next beat
                        clickable = true;
                    }
                }
            }

        }
        FuelUpdate(dt);
        if (isCollidingWithReef && !IsTouchingReef())
        {
            isCollidingWithReef = false;
        }
    }
}


void Ship::Draw(DrawLayer drawlayer)
{
    CS230::GameObject::Draw(DrawLayer::DrawLast);
}

void Ship::SetDest()
{
    if (clickable && !set_dest) {
        if (Engine::GetInput().MouseButtonJustPressed(SDL_BUTTON_LEFT) && beat->GetIsOnBeat()) {
            // Get mouse position relative to the center of the screen
            vec2 window = { Engine::window_width / 2, Engine::window_height / 2 };
            vec2 mouse_pos = { (float)Engine::GetInput().GetMousePos().mouseWorldSpaceX, (float)Engine::GetInput().GetMousePos().mouseWorldSpaceY };
            vec2 pos = mouse_pos - window;
            destination.x = pos.x;
            destination.y = pos.y;
            clickable = false;
            set_dest = true;
        }
    }

    if (set_dest) { // if clicked the destination
        if (!beat->GetIsOnBeat() && !ready_to_move && !move) { // wait for next beat
            ready_to_move = true;
        }
    }

    if (ready_to_move && beat->GetBeat()) { // move when its on next beat
        direction = { destination.x - (GetPosition().x), destination.y - (GetPosition().y) };
        direction = direction.Normalize();
        force = { (direction * speed) };
        move = true;
        set_dest = false;
        ready_to_move = false;
    }
}

void Ship::Move(double dt)
{
    SetVelocity(force);
    force *= deceleration;
    //std::cout << force.x << std::endl;
    if (!beat->GetIsOnBeat()) {
        SetVelocity(direction * skidding_speed);
        if (!clickable) { // wait for next beat
            clickable = true;
        }
        move = false;
    }
}

vec2 GetPerpendicular(vec2 v) {
    return { -v.y, v.x };
}


bool Ship::CanCollideWith(GameObjectTypes other_object)
{
    switch (other_object) {
    case GameObjectTypes::Fish:
    case GameObjectTypes::Reef:
        return true;
        break;
    }

    return false;
}

void Ship::ResolveCollision(GameObject* other_object)
{
    if (other_object->Type() == GameObjectTypes::Reef) {
        auto* collision_edge = this->GetGOComponent<CS230::RectCollision>();
        if (collision_edge == nullptr) {
            // maybe an error?
        }
        HitWithReef(collision_edge);
    }
}

float Dot(const vec2& vec1, const vec2& vec2) {
    return vec1.x * vec2.x + vec1.y * vec2.y;
}

void Ship::HitWithReef(CS230::RectCollision* collision_edge) {
    fuel -= HitDecFuel;

    // 충돌하는 벽의 두 점 (시작과 끝)
    vec2 edge_1 = collision_edge->GetCollidingEdge().first;
    vec2 edge_2 = collision_edge->GetCollidingEdge().second;

    // 벽의 방향과 법선 계산
    vec2 wall_dir = { edge_2.x - edge_1.x, edge_2.y - edge_1.y };
    vec2 wall_perpendicular = GetPerpendicular(wall_dir);
    vec2 normal = wall_perpendicular.Normalize();

    // 현재 선체의 속도와 위치
    vec2 velocity = GetVelocity();
    vec2 ship_position = GetPosition();

    // TOI 계산을 위한 변수들
    float toi = 1.0f;  // 충돌 시간 비율 (0 ~ 1), 기본적으로 전체 이동
    bool isApproaching = false;

    // 벽의 벡터와 선체의 이동 벡터를 통해 TOI 계산
    vec2 relative_position = ship_position - edge_1;
    float wall_length_squared = wall_dir.x * wall_dir.x + wall_dir.y * wall_dir.y;
    float t = (relative_position.x * wall_dir.x + relative_position.y * wall_dir.y) / wall_length_squared;

    if (t >= 0.0f && t <= 1.0f) {
        // 충돌 지점이 벽 내부에 있는 경우에만 TOI를 계산합니다.
        vec2 closest_point = edge_1 + wall_dir * t;
        vec2 relative_velocity = velocity - vec2{ 0.0f, 0.0f };  // 상대적인 속도 (벽은 고정된 것으로 가정)

        // 법선 벡터와 속도의 내적을 통해 충돌 접근 여부 확인
        float relative_speed_along_normal = Dot(relative_velocity, normal);
        if (relative_speed_along_normal < 0) {
            // 접근 중인 경우에만 충돌 시점 계산
            float distance_to_collision = Dot(closest_point - ship_position, normal);
            toi = distance_to_collision / -relative_speed_along_normal;
            toi = std::clamp(toi, 0.0f, 1.0f);  // toi를 0과 1 사이로 클램프
            isApproaching = true;
        }
    }

    // 충돌이 예상되는 경우, TOI 지점으로 이동시킵니다.
    if (isApproaching && toi < 1.0f) {
        // 충돌하기 직전까지만 이동
        SetPosition(ship_position + velocity * toi);
    }
    else {
        // 충돌이 예상되지 않거나 이미 충돌한 경우에는 기존 위치에서 보정 이동
        SetPosition(ship_position + -velocity * 0.007f);
    }

    // 반사 계산 (충돌 이후 처리)
    float incoming_speed = sqrt(velocity.x * velocity.x + velocity.y * velocity.y);

    float dot_product_normal_velocity = velocity.x * normal.x + velocity.y * normal.y;
    if (dot_product_normal_velocity > 0) {
        normal = normal * -1.0f;  // 법선 반전
    }

    float dot_product = velocity.x * normal.x + velocity.y * normal.y;
    vec2 reflection = {
        velocity.x - 2 * dot_product * normal.x,
        velocity.y - 2 * dot_product * normal.y
    };

    direction = reflection.Normalize();

    // 속도 조정
    if (incoming_speed > 3300.f) incoming_speed = 3300.f;
    if (incoming_speed < 150.f) incoming_speed = 150.f;

    // 반사 속도 설정 및 위치 보정
    SetVelocity(direction * incoming_speed * 0.75f);
    SetPosition(GetPosition() + normal * 0.5f);  // 충돌 후 약간 벽에서 떨어지게 보정

    move = false;
    hit_with = true;
}

//for fuel
void Ship::FuelUpdate(double dt)
{

    bool Reduce = false;

    if (fuel <= 0)
    {
        FuelFlag = true;
    }

    if (FuelFlag == false)
    {
        fuelcounter += dt;

        if (fuelcounter >= 1) // Decreased fuel per second
        {
            fuel -= baseDecfuel;
            fuelcounter = 0;
        }

        if (move == true)
        {
            Reduce = true;
            if (Reduce == true)
            {
                fuel -= MoveDecfuel;
                Reduce = false;
            }
        }

        //std::cout << "Fuel: " << fuel << std::endl;
    }

    if (FuelFlag == true)
    {
        IsFuelZero();
    }
}

void Ship::SetMaxFuel(double input)
{
    Maxfuel = input;
}



bool Ship::IsTouchingReef()
{
    if (isCollidingWithReef == true)
    {
        return true;
    }
    return false;
}

bool Ship::IsFuelZero()
{
    return FuelFlag;
    fuel -= HitDecFuel;
    //std::cout << "Collision with Reef!" << std::endl;

}

bool Ship::IsShipUnder()
{
    if (this->GetPosition().y > 180)
    {
        return true;
    }
    return false;
}