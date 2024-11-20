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
    std::cout << (!hit_with ? "can collide" : "no") << std::endl;
    //std::cout << (!set_dest && beat->GetIsOnBeat() && !move) << std::endl;
    GameObject::Update(dt);
    CS230::RectCollision* collider = GetGOComponent<CS230::RectCollision>();

    // World Boundary
    if (collider->WorldBoundary_rect().Left() < Engine::GetGameStateManager().GetGSComponent<CS230::Cam>()->GetPosition().x - 640) {
        UpdatePosition({ Engine::GetGameStateManager().GetGSComponent<CS230::Cam>()->GetPosition().x - 640 - collider->WorldBoundary_rect().Left(), 0});
        SetVelocity({ 0, GetVelocity().y });
    }
    if (collider->WorldBoundary_rect().Right() > Engine::GetGameStateManager().GetGSComponent<CS230::Cam>()->GetPosition().x + 640) {
        UpdatePosition({ Engine::GetGameStateManager().GetGSComponent<CS230::Cam>()->GetPosition().x + 640 - collider->WorldBoundary_rect().Right(), 0 });
        SetVelocity({ 0, GetVelocity().y });
    }
    if (collider->WorldBoundary_rect().Bottom() < Engine::GetGameStateManager().GetGSComponent<CS230::Cam>()->GetPosition().y - 360) {
        UpdatePosition({ 0, Engine::GetGameStateManager().GetGSComponent<CS230::Cam>()->GetPosition().y - 360 - collider->WorldBoundary_rect().Bottom() });
        SetVelocity({ GetVelocity().x, 0 });
    }
    if (collider->WorldBoundary_rect().Top() > Engine::GetGameStateManager().GetGSComponent<CS230::Cam>()->GetPosition().y + 360) {
        UpdatePosition({ 0, Engine::GetGameStateManager().GetGSComponent<CS230::Cam>()->GetPosition().y + 360 - collider->WorldBoundary_rect().Top()});
        SetVelocity({ GetVelocity().x, 0 });
    }
}

void Ship::FixedUpdate(double fixed_dt)
{
    GameObject::FixedUpdate(fixed_dt);
    if (Engine::GetGameStateManager().GetStateName() == "Mode1") {
        SetDest();

        if (move) {
            Move(fixed_dt);

        }
        FuelUpdate(fixed_dt);
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
    float base_dt = 1.0f / 240.f;  // 기준 시간 (60FPS 기준)
    float adjusted_deceleration = (float)pow(deceleration, dt / base_dt);  // 비례 보정
    force *= adjusted_deceleration;

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

vec2 CatmullRomSpline(const vec2& p0, const vec2& p1, const vec2& p2, const vec2& p3, float t) {
    float t2 = t * t;
    float t3 = t2 * t;

    return 0.5f * ((2.0f * p1) +
        (-p0 + p2) * t +
        (2.0f * p0 - 5.0f * p1 + 4.0f * p2 - p3) * t2 +
        (-p0 + 3.0f * p1 - 3.0f * p2 + p3) * t3);
}

std::vector<vec2> GenerateSplinePoints(const std::vector<vec2>& points, int resolution) {
    std::vector<vec2> spline_points;

    if (points.size() < 4) {
        return spline_points; // 최소한 4개의 점이 필요
    }

    // Catmull-Rom 스플라인 생성
    for (int i = 0; i < resolution; ++i) {
        float t = i / static_cast<float>(resolution - 1); // 0 ~ 1로 정규화
        vec2 point = CatmullRomSpline(points[0], points[1], points[2], points[3], t);
        spline_points.push_back(point);
    }

    return spline_points;
}

std::vector<vec2> FindClosestPoints(const Polygon& polygon, const vec2& pos, int num_points) {
    std::vector<std::pair<float, vec2>> distances;

    // 각 점과의 거리 계산
    for (const auto& vertex : polygon.vertices) {
        float distance = (vertex - pos).Length();
        distances.push_back({ distance, vertex });
    }

    // 거리 순으로 정렬
    std::sort(distances.begin(), distances.end(),
        [](const std::pair<float, vec2>& a, const std::pair<float, vec2>& b) {
            return a.first < b.first;
        });

    // 가장 가까운 점 num_points개 선택
    std::vector<vec2> closest_points;
    for (int i = 0; i < num_points && i < distances.size(); ++i) {
        closest_points.push_back(distances[i].second);
    }

    return closest_points;
}

vec2 FindClosestPointOnSpline(const std::vector<vec2>& spline_points, const vec2& position) {
    float min_distance = std::numeric_limits<float>::max();
    vec2 closest_point;

    for (const auto& point : spline_points) {
        float distance = (point - position).Length();
        if (distance < min_distance) {
            min_distance = distance;
            closest_point = point;
        }
    }

    return closest_point;
}

vec2 ComputeNormalAtPoint(const vec2& p0, const vec2& p1) {
    vec2 tangent = p1 - p0;
    vec2 normal = { -tangent.y, tangent.x }; // 접선에 수직
    float length = normal.Length();
    return { normal.x / length, normal.y / length }; // 정규화
}

bool Ship::CanCollideWith(GameObjectTypes other_object)
{
    switch (other_object) {
    case GameObjectTypes::Fish:
    case GameObjectTypes::Reef:
        if (!hit_with) return true;
        else return false;
        break;
    }

    return false;
}

vec2 ComputeCollisionNormal(const Polygon& polygon, const vec2& pos, int resolution = 20) {
    // 1. 가장 가까운 4개의 점 찾기
    std::vector<vec2> closest_points = FindClosestPoints(polygon, pos, 4);

    // 2. 스플라인 생성
    std::vector<vec2> spline_points = GenerateSplinePoints(closest_points, resolution);

    // 3. 스플라인 위에서 EntryData의 위치와 가장 가까운 점 찾기
    vec2 closest_point_on_spline = FindClosestPointOnSpline(spline_points, pos);

    // 4. 충돌 지점에서 법선 벡터 계산
    size_t closest_index = std::find(spline_points.begin(), spline_points.end(), closest_point_on_spline) - spline_points.begin();
    vec2 tangent_point1 = spline_points[std::max(0, static_cast<int>(closest_index) - 1)];
    vec2 tangent_point2 = spline_points[std::min(static_cast<int>(spline_points.size() - 1), static_cast<int>(closest_index) + 1)];
    vec2 normal = ComputeNormalAtPoint(tangent_point1, tangent_point2);

    return normal;
}

void Ship::ResolveCollision(GameObject* other_object)
{
    if (!hit_with) {
        if (other_object->Type() == GameObjectTypes::Reef) {
            Polygon poly = other_object->GetGOComponent<CS230::MAP_SATCollision>()->WorldBoundary_poly();
            vec2 normal = ComputeCollisionNormal(poly, GetPosition());

            auto* collision_edge = this->GetGOComponent<CS230::RectCollision>();
            if (collision_edge == nullptr) {
                // maybe an error?
            }

            HitWithReef(normal);
        }
    }
}

float Dot(const vec2& vec1, const vec2& vec2) {
    return vec1.x * vec2.x + vec1.y * vec2.y;
}


void Ship::HitWithReef(vec2 normal) {
    fuel -= HitDecFuel;
    

    vec2 velocity = GetVelocity();
    float dot_product = velocity.x * normal.x + velocity.y * normal.y;
    vec2 reflection = {
        velocity.x - 2 * dot_product * normal.x,
        velocity.y - 2 * dot_product * normal.y
    };

    direction = reflection.Normalize();

    // Current velocity and position of the ship

    // Set reflection speed and adjust position
    float incoming_speed = sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
    SetVelocity(direction * incoming_speed * 0.75f);
    //SetVelocity({});
    //SetPosition(GetPosition() + normal * 0.5f);  // Adjust slightly to move away from the wall after collision

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