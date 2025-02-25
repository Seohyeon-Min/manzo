#include "Ship.h"
#include "BeatSystem.h"
#include "Particles.h"
#include "../Engine/Camera.h"
#include "../Engine/Input.h"
#include "../Engine/MapManager.h"
#include "angles.h"
#include "Effect.h"
#include "WindowState.h"

#include <iostream>

Ship::Ship(vec2 start_position) :
    GameObject(start_position), move(false)
{
    AddGOComponent(new Sprite("assets/images/ship.spt", this));
    beat = Engine::GetGameStateManager().GetGSComponent<Beat>();
    skill = Engine::GetGameStateManager().GetGSComponent<Skillsys>();

    fuel = Maxfuel;
    FuelFlag = false;
    SetVelocity({ 0,0 });

    if (Engine::GetGameStateManager().GetStateName() == "Mode1") {
        Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->Add(new Pump);
        current_state = &state_idle;
        current_state->Enter(this);
        fuel_bubble_timer = new Timer(0.0);
        AddGOComponent(fuel_bubble_timer);
        collide_timer = new RealTimeTimer(collide_time);
        AddGOComponent(collide_timer);
        fuel_bubble_timer->Set(fuel_bubble_time);
    }
}

void Ship::State_Idle::Enter(GameObject* object) {
    Ship* ship = static_cast<Ship*>(object);
    if (ship->GetVelocity() != vec2{})
        ship->SetVelocity(ship->direction * skidding_speed);
}
void Ship::State_Idle::Update([[maybe_unused]] GameObject* object, [[maybe_unused]] double dt) {
    //get mouse pos and move to there
    Ship* ship = static_cast<Ship*>(object);

    Engine::GetInput().GetMousePos().mouseWorldSpaceX - Engine::window_width / 2 > ship->GetPosition().x ? ship->SetFlipX(true) : ship->SetFlipX(false);

    vec2 ship_position = ship->GetPosition();
    vec2 window = { Engine::window_width / 2, Engine::window_height / 2 };
    vec2 mouse_pos = { (float)Engine::GetInput().GetMousePos().mouseWorldSpaceX, (float)Engine::GetInput().GetMousePos().mouseWorldSpaceY };
    vec2 pos = mouse_pos - window;


    vec2 destination = pos;
    vec2 direction = destination - ship_position;
    float distance = direction.Length();
    direction = direction.Normalize();


    float force_multiplier = 0.0f;
    float min_distance = 50.0f;
    float max_distance = 200.0f;

    if (distance <= min_distance) {
        force_multiplier = 0.0f;
    }
    else if (distance >= max_distance) {
        force_multiplier = 1.0f;
    }
    else {
        force_multiplier = (distance - min_distance) / (max_distance - min_distance);
    }


    vec2 force = direction * skidding_speed * force_multiplier;

    float randomAngle = util::random(180.0f, 200.0f);
    float angleRadians = util::to_radians(randomAngle);
    vec2 bubble_direction = { cos(angleRadians), sin(angleRadians) };
    vec2 target_position = {
    ship_position.x + bubble_direction.x * (-30.f * (ship->GetFlipX() ? -1.f : 1.f)),
    ship_position.y + bubble_direction.y * -30.f
    };

    ship->SetVelocity(force);
    
    if (ship->fuel_bubble_timer->Remaining() == 0.0 && force_multiplier > 0.4) {
        Engine::GetGameStateManager().GetGSComponent<ParticleManager<Particles::FuelBubble>>()->Emit(1, target_position, { 0,0 }, -force * 0.4f, 1.5);
        ship->fuel_bubble_timer->Reset();
    }
}
void Ship::State_Idle::CheckExit(GameObject* object) {
    Ship* ship = static_cast<Ship*>(object);
    //if (ship->hit_with) {
    //    ship->change_state(&ship->state_hit);
    //}
    if (ship->can_dash && Engine::GetInput().MouseButtonJustPressed(SDL_BUTTON_LEFT) && ship->beat->GetIsOnBeat()) {
        // Get mouse position relative to the center of the screen
        vec2 window = { Engine::window_width / 2, Engine::window_height / 2 };
        vec2 mouse_pos = { (float)Engine::GetInput().GetMousePos().mouseWorldSpaceX, (float)Engine::GetInput().GetMousePos().mouseWorldSpaceY };
        vec2 pos = mouse_pos - window;
        ship->destination.x = pos.x;
        ship->destination.y = pos.y;
        ship->direction = { ship->destination.x - (ship->GetPosition().x), ship->destination.y - (ship->GetPosition().y) };
        ship->direction = ship->direction.Normalize();
        ship->force = { (ship->direction * speed) };
        ship->change_state(&ship->state_move);
    }
}


void Ship::State_Move::Enter(GameObject* object) {
    Ship* ship = static_cast<Ship*>(object);
    ship->move = true;
    vec2 dir = ship->GetVelocity().Normalize();
    Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->Add(new DashEffect());
    
}
void Ship::State_Move::Update([[maybe_unused]] GameObject* object, [[maybe_unused]] double dt) {
    Ship* ship = static_cast<Ship*>(object);

}

void Ship::State_Move::FixedUpdate([[maybe_unused]] GameObject* object, [[maybe_unused]] double fixed_dt) {
    Ship* ship = static_cast<Ship*>(object);
    if (!ship->hit_with) {
        ship->FuelUpdate(fixed_dt);
        ship->SetVelocity(ship->force);
        float base_dt = 1.0f / 240.f;
        float adjusted_deceleration = (float)pow(deceleration, fixed_dt / base_dt);
        ship->force *= adjusted_deceleration;
    }
    ship->nearestRock = Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->FindNearestRock(ship); // it should be FindNearestRockNextFrame

#ifdef _DEBUG
    if (ship->nearestRock) {
        if (ship->before_nearest_rock && ship->before_nearest_rock != ship->nearestRock) {
            ship->before_nearest_rock->GetRockGroup()->SetShader(Engine::GetShaderManager().GetDefaultShader());
        }
        else {
            ship->nearestRock->GetRockGroup()->SetShader(Engine::GetShaderManager().GetShader("purple"));
        }
        ship->before_nearest_rock = ship->nearestRock;
    }

    if (Engine::GetInput().KeyJustPressed(Input::Keys::G)) {
        ship->change_state(&ship->state_idle);
        ship->hit_with = false;
    }
#else
    if (ship->nearestRock) {
        ship->before_nearest_rock = ship->nearestRock;
    }
#endif

    if (ship->nearestRock == NULL) {
        Engine::GetLogger().LogEvent("Nearest Rock : NULL");
        return;
    }

    if (ship->IsCollidingWithNextFrame(ship->before_nearest_rock, ship->GetVelocity(), (float)fixed_dt, ship->toi)) {
        Engine::GetLogger().LogEvent("Collision Detected, Its toi is : " + std::to_string(ship->toi));
        //vec2 smallCorrection = -ship->GetVelocity().Normalize();
        if (ship->toi <= 0) { // just in case
            ship->SetPosition(ship->GetPosition() - ship->GetVelocity() * (float)fixed_dt);
            Engine::GetLogger().LogEvent("Toi is 0, returned. : " + std::to_string(ship->toi));
            return;
        }
        ship->should_resolve_collision = true;
        ship->hit_with = true;
    }
    if (ship->should_resolve_collision) {
        vec2 velocity = ship->GetVelocity();
        ship->collisionPos = ship->GetPosition() + velocity * (float)fixed_dt * ship->toi;
        ship->SetVelocity({});
        ship->SetPosition(ship->collisionPos); // Bug: it doens't really stop at right pos
        Engine::GetLogger().LogEvent("@@@@ In resolve_collision @@@@");
        //this should be started in a next frame
        //ship->nearestRock = Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->FindNearestRock(ship); // it should be FindNearestRockNextFrame
        if (ship->before_nearest_rock) {
            ship->collide_timer->Start();
            ship->HitWithRock(ship->nearestRock); // calculate normal

            Engine::GetLogger().LogEvent("Velocity: " + std::to_string(velocity.x) + ", " + std::to_string(velocity.y));
            Engine::Instance().SetSlowDownFactor(ship->slow_down_factor);
            ship->direction = ship->CalculateHitDirection(ship->normal, velocity);

            float speed = velocity.Length();

            if (speed >= 6000.f) {
                speed = 6000.f - ship->slow_down;
                ship->slow_down += 1500.f;
                std::cout << "ship->slow_down: " << ship->slow_down << std::endl;
            }

            ship->force = ship->direction * speed * 0.8f;
            ship->force *= 7.0f;
            Engine::GetLogger().LogEvent("velocity.Length(), speed : " + std::to_string(velocity.Length()) + " : " + std::to_string(speed));
            Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->Add(new HitEffect(ship->GetPosition()));
        }
        else {
            Engine::GetLogger().LogEvent("@@@@ Error: No nearest rock!! @@@@");
        }
        ship->should_resolve_collision = false;
    }
}
void Ship::State_Move::CheckExit(GameObject* object) {
    Ship* ship = static_cast<Ship*>(object);
    if (!ship->beat->GetIsOnBeat() && !ship->hit_with) {
        ship->move = false;
        ship->change_state(&ship->state_idle);
    }
}

std::vector<vec2> spline_points;


void Ship::Update(double dt)
{
    GameObject::Update(dt);

    can_dash = true;
    if (Engine::GetGameStateManager().GetStateName() == "Mode1") {

        if (collide_timer->Remaining() < 0.48) {
            Engine::GetGameStateManager().GetGSComponent<Cam>()->GetCameraView().SetZoom(1.0f);
            SetVelocity(force);
            float base_dt = 1.0f / 240.f;
            float adjusted_deceleration = (float)pow(deceleration / 2, dt / base_dt);
            force *= adjusted_deceleration;
        }

        if (collide_timer->IsFinished()) {
            Engine::Instance().ResetSlowDownFactor();
            collide_timer->Reset();
            slow_down = 0.0f;
            hit_with = false;
        }

        Engine::GetGameStateManager().GetGSComponent<Cam>()->GetCamera().UpdateShake((float)dt);
        // World Boundary
        RectCollision* collider = GetGOComponent<RectCollision>();

        if (collider->WorldBoundary_rect().Left() < Engine::GetGameStateManager().GetGSComponent<Cam>()->GetPosition().x - 640) {
            UpdatePosition({ Engine::GetGameStateManager().GetGSComponent<Cam>()->GetPosition().x - 640 - collider->WorldBoundary_rect().Left(), 0 });
            SetVelocity({ 0, GetVelocity().y });
        }
        if (collider->WorldBoundary_rect().Right() > Engine::GetGameStateManager().GetGSComponent<Cam>()->GetPosition().x + 640) {
            UpdatePosition({ Engine::GetGameStateManager().GetGSComponent<Cam>()->GetPosition().x + 640 - collider->WorldBoundary_rect().Right(), 0 });
            SetVelocity({ 0, GetVelocity().y });
        }
        if (collider->WorldBoundary_rect().Bottom() < Engine::GetGameStateManager().GetGSComponent<Cam>()->GetPosition().y - 360) {
            UpdatePosition({ 0, Engine::GetGameStateManager().GetGSComponent<Cam>()->GetPosition().y - 360 - collider->WorldBoundary_rect().Bottom() });
            SetVelocity({ GetVelocity().x, 0 });
        }
        if (collider->WorldBoundary_rect().Top() > Engine::GetGameStateManager().GetGSComponent<Cam>()->GetPosition().y + 360) {
            UpdatePosition({ 0, Engine::GetGameStateManager().GetGSComponent<Cam>()->GetPosition().y + 360 - collider->WorldBoundary_rect().Top() });
            SetVelocity({ GetVelocity().x, 0 });
        }

        if (isCollidingWithReef && !IsTouchingReef())
        {
            isCollidingWithReef = false;
        }
    }
    //std::cout << Engine::GetInput().GetMousePos().mouseWorldSpaceX - Engine::window_width/ 2 << " " << Engine::GetInput().GetMousePosition().x << std::endl;
}

void Ship::FixedUpdate(double fixed_dt) {
    GameObject::FixedUpdate(fixed_dt);



    //if (nearestRock) {
    //    float t_hit;
    //    MAP_SATCollision* rockCollider = nearestRock->GetGOComponent<MAP_SATCollision>();

    //    //std::cout << "There is a nearest rock at " << nearestRock->GetPosition().x << std::endl;
    //    if (ComputeTOIWithRock(currentPos, velocity, rockCollider, t_hit)) {
    //        vec2 hitPos = currentPos + velocity * t_hit;  // 충돌 지점까지만 이동
    //        SetPosition(hitPos); // 경계선에서 멈춤
    //        //std::cout << "exist" << hitPos.x << " : " << hitPos.y << std::endl;
    //        // **속도를 0으로 하여 완전 정지**
    //        SetVelocity({ 0, 0 });
    //        return;
    //    }
    //}
}


void Ship::Draw(DrawLayer drawlayer) {
    GameObject::Draw(DrawLayer::DrawPlayer);
}

vec2 CatmullRomSpline(const vec2& p0, const vec2& p1, const vec2& p2, const vec2& p3, float t) {
    float t2 = t * t;
    float t3 = t2 * t;

    return 0.5f * ((2.0f * p1) +
        (-p0 + p2) * t +
        (2.0f * p0 - 5.0f * p1 + 4.0f * p2 - p3) * t2 +
        (-p0 + 3.0f * p1 - 3.0f * p2 + p3) * t3);
}

std::vector<vec2> SortPointsCounterClockwise(const std::vector<vec2>& points, const vec2& center) {
    std::vector<vec2> sorted_points = points;

    std::sort(sorted_points.begin(), sorted_points.end(),
        [&center](const vec2& a, const vec2& b) {
            float angle_a = atan2(a.y - center.y, a.x - center.x);
            float angle_b = atan2(b.y - center.y, b.x - center.x);
            return angle_a > angle_b;
        });

    return sorted_points;
}

std::vector<vec2> ExtendBoundaryPoints(const std::vector<vec2>& points) {
    std::vector<vec2> extended_points = points;

    if (points.size() > 3) {

        extended_points.insert(extended_points.begin(), points.back());
        extended_points.push_back(points.front());
        extended_points.push_back(points[1]);
    }

    return extended_points;
}

std::vector<vec2> GenerateSplinePoints(const std::vector<vec2>& points, int resolution) {
    std::vector<vec2> spline_points;

    if (points.size() < 2) {
        return spline_points;
    }

    std::vector<vec2> extended_points = ExtendBoundaryPoints(points);

    for (size_t i = 0; i < extended_points.size() - 3; ++i) {
        for (int j = 0; j < resolution; ++j) {
            float t = j / static_cast<float>(resolution - 1);
            vec2 point = CatmullRomSpline(extended_points[i], extended_points[i + 1], extended_points[i + 2], extended_points[i + 3], t);
            spline_points.push_back(point);
        }
    }

    return spline_points;
}

vec2 FindClosestPointOnSpline(const std::vector<vec2>& spline_points, const vec2& position) {
    float min_distance = std::numeric_limits<float>::max();
    vec2 closest_point;
    size_t closest_index = 0;

    for (size_t i = 0; i < spline_points.size(); ++i) {
        float distance = (spline_points[i] - position).Length();
        if (distance < min_distance) {
            min_distance = distance;
            closest_point = spline_points[i];
            closest_index = i;
        }
    }

    return closest_point;
}

vec2 ComputeNormalAtPoint(const vec2& p0, const vec2& p1) {
    vec2 tangent = p1 - p0;
    vec2 normal = { -tangent.y, tangent.x };
    float length = normal.Length();
    if (length > 0.0f) {
        normal = { normal.x / length, normal.y / length };

        if (normal.x < 0.0f) {
            normal.x = -normal.x;
            normal.y = -normal.y;
        }

        return normal;
    }
    return { 0.0f, 0.0f };
}

vec2 ComputeCollisionNormal(const std::vector<vec2>& points, const vec2& pos, const vec2& center, int resolution = 20) {
    std::vector<vec2> closest_points = points;
    std::vector<vec2> sorted_point = SortPointsCounterClockwise(closest_points, center);

    spline_points = GenerateSplinePoints(sorted_point, resolution);

    vec2 closest_point_on_spline = FindClosestPointOnSpline(spline_points, pos);

    size_t closest_index = std::find(spline_points.begin(), spline_points.end(), closest_point_on_spline) - spline_points.begin();
    vec2 tangent_point1 = spline_points[std::max(0, static_cast<int>(closest_index) - 1)];
    vec2 tangent_point2 = spline_points[std::min(static_cast<int>(spline_points.size() - 1), static_cast<int>(closest_index) + 1)];
    vec2 normal = ComputeNormalAtPoint(tangent_point1, tangent_point2);

    Engine::GetLogger().LogEvent("Normal: " + std::to_string(normal.x) + ", " + std::to_string(normal.y));
    return normal;
}

bool Ship::CanCollideWith(GameObjectTypes other_object)
{
    switch (other_object) {
    case GameObjectTypes::Fish:
    case GameObjectTypes::Rock:
        return true;
        break;
    }

    return false;
}

void Ship::ResolveCollision(GameObject* other_object)
{
    if (other_object->Type() == GameObjectTypes::Fish) {
        change_state(&state_idle);
    }

    if (other_object->Type() == GameObjectTypes::Rock) {

        if (GetVelocity().Length() <= skidding_speed + 30.f) { // if it was skidding, don't reflect
            vec2 smallCorrection = -GetVelocity().Normalize(); // with this, ship should not able to move!
            UpdatePosition(smallCorrection);
            can_dash = false;
            return;
        }
    }
    
}

void Ship::HitWithRock(Rock* rock)
{
    fuel -= HitDecFuel;

    if (fuel < 0.0f) {
        fuel = 0.0f;
    }

    auto cam = Engine::GetGameStateManager().GetGSComponent<Cam>();
    //cam->GetCameraView().SetZoom(1.05f);
    cam->GetCamera().StartShake(camera_shake, 5);

    std::vector<vec2> points = rock->GetRockGroup()->GetPoints();
    vec2 center = rock->GetRockGroup()->FindCenterPoly();

    normal = ComputeCollisionNormal(points, GetPosition(), center);
}


vec2 Ship::CalculateHitDirection(vec2 normal, vec2 velocity) {
    float dot_product = velocity.x * normal.x + velocity.y * normal.y;

    vec2 direction;

    vec2 reflection = {
        velocity.x - 2 * dot_product * normal.x,
        velocity.y - 2 * dot_product * normal.y
    };

    float incoming_speed = velocity.Length();

    if (reflection.Length() > 0.0f) {
        direction = reflection.Normalize();
    }
    else {
        direction = { 1.0f, 0.0f };
    }

    move = false;

    Engine::GetLogger().LogEvent("Direction: " + std::to_string(direction.x) + ", " + std::to_string(direction.y));
    return direction;

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


Pump::Pump() :
    GameObject({})
{
    beat = Engine::GetGameStateManager().GetGSComponent<Beat>();
}

void Pump::Update(double dt)
{
    GameObject::Update(dt);
    Ship* ship = Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->GetGOComponent<Ship>();
    SetPosition(ship->GetPosition());
    GetMatrix();

    float decrease_duration = (float)beat->GetFixedDuration() - 0.1f;
    float delta_radius = (max_pump_radius - min_pump_radius) / decrease_duration;
    float delta_alpha = 1 / decrease_duration;

    if (beat->GetBeat()) {
        radius = min_pump_radius;
        wait = true;
    }
    if (wait && !beat->GetIsOnBeat()) {
        radius = max_pump_radius;
        alpha = 0.f;
        wait = false;
    }

    if (!wait && radius > min_pump_radius) {
        radius -= delta_radius * (float)dt;
        alpha += delta_alpha * (float)dt;
    }

    radius = std::max(radius, min_pump_radius);
}

void Pump::Draw(DrawLayer drawlayer)
{
    CircleDrawCall draw_call = {
    min_pump_radius,                       // Texture to draw
    GetPosition(),                          // Transformation matrix
    };

    draw_call.settings.do_blending = true;
    draw_call.sorting_layer = DrawLayer::DrawUI;

    CircleDrawCall draw_call2 = {
    radius,                       // Texture to draw
    GetPosition(),                          // Transformation matrix
    };

    draw_call2.shader = Engine::GetShaderManager().GetShader("change_alpha_no_texture"); // Shader to use
    draw_call2.settings.do_blending = true;
    draw_call2.SetUniforms = [this](const GLShader* shader) {this->SetUniforms(shader);};
    draw_call2.sorting_layer = DrawLayer::DrawUI;

    Engine::GetRender().AddDrawCall(std::make_unique<CircleDrawCall>(draw_call));
    Engine::GetRender().AddDrawCall(std::make_unique<CircleDrawCall>(draw_call2));
}

void Pump::Reset() {
    radius = max_pump_radius;
    alpha = 0.0f;
    wait = false;
}


void Pump::SetUniforms(const GLShader* shader)
{
    shader->SendUniform("uAlpha", alpha);
}