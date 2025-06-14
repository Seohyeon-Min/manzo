#include "Ship.h"
#include "Particles.h"
#include "../Engine/Camera.h"
#include "../Engine/Input.h"
#include "../Engine/MapManager.h"
#include "angles.h"
#include "Effect.h"
#include "WindowState.h"
#include "Monster.h"
#include "States.h"

#include <iostream>

Ship::Ship(vec2 start_position) :
	GameObject(start_position), move(false)
{
	AddGOComponent(new Sprite("assets/images/ship/ship.spt", this));
	beat = &Engine::GetBeat();
	hit_text = Engine::GetTextureManager().Load("assets/images/ship/ship_hit.png");

	fuel = Maxfuel;
	FuelFlag = false;
	SetVelocity({ 0,0 });

	if (Engine::GetGameStateManager().GetStateName() == "Mode3" || Engine::GetGameStateManager().GetStateName() == "Mode1") {
		bounceBehavior = new DefaultBounceBehavior();
		Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->Add(new Pump);
		current_state = &state_idle;
		current_state->Enter(this);
		fuel_bubble_timer = new Timer(0.0);
		AddGOComponent(fuel_bubble_timer);
		invincibility_timer = new Timer(0.0);
		AddGOComponent(invincibility_timer);
		collide_timer = new RealTimeTimer(collide_time);
		AddGOComponent(collide_timer);
		fuel_bubble_timer->Set(fuel_bubble_time);
	}
	if (Engine::GetGameStateManager().GetStateName() == "Tutorial")
	{
		bounceBehavior = new DefaultBounceBehavior();
		Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->Add(new Pump);
		current_state = &state_tutorial;
		current_state->Enter(this);
		fuel_bubble_timer = new Timer(0.0);
		AddGOComponent(fuel_bubble_timer);
		invincibility_timer = new Timer(0.0);
		AddGOComponent(invincibility_timer);
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

	ship->dash_target = pos;
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


	ship->dash_target = ship_position + direction * max_distance;

	ship->SetVelocity(force);

	if (ship->fuel_bubble_timer->Remaining() == 0.0 && force_multiplier > 0.4) {
		Engine::GetGameStateManager().GetGSComponent<ParticleManager<Particles::FuelBubble>>()->Emit(1, target_position, { 0,0 }, -force * 0.4f, 1.5);
		ship->fuel_bubble_timer->Reset();
	}
}
void Ship::State_Idle::CheckExit(GameObject* object) {
	Ship* ship = static_cast<Ship*>(object);
	if (ship->can_dash && Engine::GetInput().MouseButtonJustPressed(SDL_BUTTON_LEFT) && ship->beat->GetIsOnBeat()) {
		ship->destination = ship->dash_target;
		ship->direction = { ship->destination.x - ship->GetPosition().x,
							ship->destination.y - ship->GetPosition().y };
		ship->direction = ship->direction.Normalize();
		ship->force = ship->direction * speed;

		ship->change_state(&ship->state_move);

		ship->dash_success = true;
	}
}

void Ship::State_Move::Enter(GameObject* object) {
	Ship* ship = static_cast<Ship*>(object);
	ship->move = true;
	vec2 dir = ship->GetVelocity().Normalize();

	if (skip_enter) return;
	Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->Add(new DashEffect());
}
void Ship::State_Move::Update([[maybe_unused]] GameObject* object, [[maybe_unused]] double dt) {
	Ship* ship = static_cast<Ship*>(object);
	ship->dash_success = false;
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
		//Engine::GetLogger().LogEvent("Nearest Rock : NULL");
		return;
	}
	vec2 velocity;

	if (ship->IsCollidingWithNextFrame(ship->nearestRock, ship->GetVelocity(), (float)fixed_dt, ship->toi)) {
		Engine::GetLogger().LogEvent("Collision Detected, Its toi is : " + std::to_string(ship->toi));

		if (ship->toi <= 0) { // just in case
			ship->SetPosition(ship->GetPosition() - ship->GetVelocity() * (float)fixed_dt);
			Engine::GetLogger().LogEvent("Toi is 0, returned. : " + std::to_string(ship->toi));
			return;
		}
		ship->should_resolve_collision = true;
		ship->hit_with = true;
		velocity = ship->GetVelocity();
	}
	if (ship->should_resolve_collision) {
		ship->SetVelocity({});
		ship->collisionPos = ship->GetPosition() + velocity * (float)fixed_dt * (ship->toi);
		vec2 smallCorrection = -velocity.Normalize();
		ship->SetPosition(ship->collisionPos + smallCorrection); // Bug: it doens't really stop at right pos
		Engine::GetLogger().LogEvent("@@@@ In resolve_collision @@@@");
		//this should be started in a next frame
		//ship->nearestRock = Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->FindNearestRock(ship); // it should be FindNearestRockNextFrame
		if (ship->nearestRock) {
			

			if (!ship->collide_timer->IsRunning()) {
				ship->collide_timer->Start();
			}
			
			ship->HitWithBounce(ship->nearestRock, velocity); // calculate normal

			if (ship->nearestRock->Type() == GameObjectTypes::ObstacleRock) {	// if obstaclerock
				ship->nearestRock->GetRockGroup()->Crash(true);		// rock is crashed (MapManager will destroy this)
			}
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
		skip_enter = false;
	}
}


void Ship::State_Die::Enter(GameObject* object)
{
	Ship* ship = static_cast<Ship*>(object);
	ship->SetVelocity({});
	Engine::Instance()->SetSlowDownFactor(ship->slow_down_factor);
	Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->Add(new BlackOutEffect());
}

void Ship::State_Die::Update(GameObject* object, double dt)
{
	Ship* ship = static_cast<Ship*>(object);
	Engine::Instance()->SetSlowDownFactor(ship->slow_down_factor * 0.1f);
	ship->SetVelocity({});
}

void Ship::State_Die::CheckExit(GameObject* object)
{
}

void Ship::State_Tutorial::Enter(GameObject* object)
{

}

void Ship::State_Tutorial::Update(GameObject* object, double dt)
{

}

void Ship::State_Tutorial::CheckExit(GameObject* object)
{

}

std::vector<vec2> spline_points;


void Ship::Update(double dt)
{
	if (!IsFuelZero()) {
		can_dash = true;
		GameObject::Update(dt);
		if (Engine::GetGameStateManager().GetStateName() == "Mode1" || Engine::GetGameStateManager().GetStateName() == "Tutorial") {

			if (collide_timer->Remaining() < 0.48) {
				// there's gonna be an error if the bgm changes

				Engine::GetGameStateManager().GetGSComponent<Cam>()->GetCameraView().SetZoom(1.0f);
				SetVelocity(force);
				float base_dt = 1.0f / 240.f;
				float adjusted_deceleration = (float)pow(deceleration / 2, dt / base_dt);
				force *= adjusted_deceleration;
			}

			if (collide_timer->IsFinished()) {
				Engine::Instance()->ResetSlowDownFactor();

				collide_timer->Reset();
				slow_down = 0.0f;
				hit_with = false;
			}

			if (invincibility_timer->Remaining() < 1.48 && invincibility_timer->TickTock() && !invincibility_timer->IsFinished()) {
				SetShader(Engine::GetShaderManager().GetShader("change_color"));
			}
			else {
				SetShader(Engine::GetShaderManager().GetDefaultShader());
			}

			Engine::GetGameStateManager().GetGSComponent<Cam>()->GetCamera().UpdateShake((float)dt);

			if (isCollidingWithReef && !IsTouchingReef())
			{
				isCollidingWithReef = false;
			}
		}

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
	}
	//std::cout << Engine::GetInput().GetMousePos().mouseWorldSpaceX - Engine::window_width/ 2 << " " << Engine::GetInput().GetMousePosition().x << std::endl;

		if (current_state == &state_move)
		{
			if (!soundPlaying)
			{
				Engine::GetAudioManager().PlayMusics("dash");
				soundPlaying = true;
			}
		}

		if (Engine::GetAudioManager().IsMusicFinished("dash"))
		{
			soundPlaying = false;
			Engine::GetAudioManager().StopPlayingMusic("dash");
		}
}

void Ship::FixedUpdate(double fixed_dt) {
	if (!IsFuelZero())
		GameObject::FixedUpdate(fixed_dt);


}


void Ship::Draw(DrawLayer drawlayer) {

	if (hit_with) {
		DrawCall draw_call = {
		hit_text,                       // Texture to draw
		&GetMatrix(),                          // Transformation matrix
		GetShader()
		};

		draw_call.settings.do_blending = true;
		draw_call.sorting_layer = DrawLayer::DrawPlayer;
		GameObject::Draw(draw_call);
	}
	else {
		DrawCall draw_call = {
		GetGOComponent<Sprite>()->GetTexture(),                       // Texture to draw
		&GetMatrix(),                          // Transformation matrix
		GetShader()
		};

		draw_call.settings.do_blending = true;
		draw_call.sorting_layer = DrawLayer::DrawPlayer;
		GameObject::Draw(draw_call);
	}




}

vec2 CatmullRomSpline(const vec2& p0, const vec2& p1, const vec2& p2, const vec2& p3, float t) {
	float tension = 0.5;
	// centripetal parameterization alpha (0.5)
	float alpha = 0.5f;

	// Compute parameter values based on distances raised to alpha
	float t0 = 0.f;
	float t1 = t0 + pow((p1 - p0).Length(), alpha);
	float t2 = t1 + pow((p2 - p1).Length(), alpha);
	float t3 = t2 + pow((p3 - p2).Length(), alpha);

	// Map t from [0,1] to the interval [t1, t2]
	float t_actual = t1 + t * (t2 - t1);

	// Compute tangent vectors with tension adjustment
	// (1 - tension): tension=0 -> standard, tension=1 -> zero tangent
	vec2 T1 = (1 - tension) * (p2 - p0) / (t2 - t0);
	vec2 T2 = (1 - tension) * (p3 - p1) / (t3 - t1);

	// Normalize the parameter to [0,1] for the Hermite basis functions
	float u = (t_actual - t1) / (t2 - t1);
	float u2 = u * u;
	float u3 = u2 * u;

	// Hermite basis functions
	float h00 = 2 * u3 - 3 * u2 + 1;
	float h10 = u3 - 2 * u2 + u;
	float h01 = -2 * u3 + 3 * u2;
	float h11 = u3 - u2;

	return h00 * p1 + h10 * (t2 - t1) * T1 + h01 * p2 + h11 * (t2 - t1) * T2;
}

std::vector<vec2> SortPointsCounterClockwise(std::span<const vec2> points, const vec2& center) {
	std::vector<vec2> sorted_points(points.begin(), points.end());
	std::sort(sorted_points.begin(), sorted_points.end(),
		[&center](const vec2& a, const vec2& b) {
			float angle_a = atan2(a.y - center.y, a.x - center.x);
			float angle_b = atan2(b.y - center.y, b.x - center.x);
			return angle_a > angle_b;
		});

	return sorted_points;
}

std::vector<vec2> ExtendBoundaryPoints(std::span<const vec2> points) {
	std::vector<vec2> extended_points(points.begin(), points.end());

	if (points.size() > 2) {

		extended_points.insert(extended_points.begin(), points.back());
		extended_points.push_back(points.front());
		extended_points.push_back(points[1]);
	}

	return extended_points;
}

std::vector<vec2> GenerateSplinePoints(std::span<const vec2> points, int resolution) {
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

vec2 FindClosestPointOnSpline(std::span<const vec2> spline_points, const vec2& position) {
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

void DrawSpline(const std::vector<vec2>& spline_points, color3 color, float width, float alpha, const GLShader* shader) {
	if (spline_points.size() < 2) return;

	for (size_t i = 0; i < spline_points.size() - 1; ++i) {
		Engine::GetRender().AddDrawCall(spline_points[i], spline_points[i + 1], color, width, alpha, shader);
	}
}

vec2 ComputeCollisionNormal(std::span<const vec2> points, const vec2& pos, const vec2& center, int resolution = 20) {
	auto sorted_point = SortPointsCounterClockwise(points, center);

	auto spline_points = GenerateSplinePoints(sorted_point, resolution);
	DrawSpline(spline_points, { 1.0f, 0.0f, 0.0f }, 2.0f, 1.0f, nullptr);

	const vec2& closest_point_on_spline = FindClosestPointOnSpline(spline_points, pos);


	size_t closest_index = std::distance(spline_points.begin(),
		std::find(spline_points.begin(), spline_points.end(), closest_point_on_spline));

	size_t index1 = (closest_index == 0) ? 0 : closest_index - 1;
	size_t index2 = (closest_index == spline_points.size() - 1) ? closest_index : closest_index + 1;

	vec2 normal = ComputeNormalAtPoint(spline_points[index1], spline_points[index2]);

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
    case GameObjectTypes::ObstacleRock:
        return true;
        break;
    case GameObjectTypes::Monster:
        if(invincibility_timer->IsFinished())
        return true;
        break;
    case GameObjectTypes::Mouse:
        if (Engine::GetGameStateManager().GetStateName() == "Mode2") {
            return true;
        }
        break;
    case GameObjectTypes::BossBullet:
        return true;
        break;
    }

	return false;
}

void Ship::ResolveCollision(GameObject* other_object) {

    switch (other_object->Type()) {
    case GameObjectTypes::Fish:
        Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->Add(new GetFishEffect(GetPosition()));
        change_state(&state_idle);
        break;
    case GameObjectTypes::Rock:
        if (GetVelocity().Length() <= skidding_speed + 10.f) { // if it was skidding, don't reflect
            vec2 smallCorrection = -GetVelocity().Normalize(); // with this, ship should not able to move!
            UpdatePosition(smallCorrection);
            can_dash = false;
            return;
        }
        break;
    case GameObjectTypes::ObstacleRock:
        if (GetVelocity().Length() <= skidding_speed + 10.f) { // if it was skidding, don't reflect
            vec2 smallCorrection = -GetVelocity().Normalize(); // with this, ship should not able to move!
            UpdatePosition(smallCorrection);
            can_dash = false;
            return;
        }
        break;
    case GameObjectTypes::Monster:
        hit_with = true;

		if (!collide_timer->IsRunning()) {
			collide_timer->Start();
		}
		HitWithBounce(other_object, -other_object->GetVelocity() * 10.f);
		if (current_state != &state_move) {
			state_move.skip_enter = true;
			change_state(&state_move);
		}
		invincibility_timer->Set(invincibility_time);
		break;
	case GameObjectTypes::Mouse:
		if (Engine::GetInput().MouseButtonJustReleased((SDL_BUTTON_LEFT))) {
			Engine::GetGameStateManager().SetNextGameState(static_cast<int>(States::Mode1));
		}
		break;
	case GameObjectTypes::BossBullet:
		ReduceFuel(BossBulletHitDecFuel);
		break;
	}

}

void Ship::HitWithBounce(GameObject* other_object, vec2 velocity) {

	if (other_object->Type() == GameObjectTypes::Rock) {

		auto cam = Engine::GetGameStateManager().GetGSComponent<Cam>();
		cam->GetCamera().StartShake(camera_shake, 5);

		Rock* rock = static_cast<Rock*>(other_object);
		std::vector<vec2> points = rock->GetPoints();
		vec2 center = rock->GetPosition();
		normal = ComputeCollisionNormal(points, GetPosition(), center);
		Engine::GetLogger().LogEvent("normal: " + std::to_string(normal.x) + ", " + std::to_string(normal.y));
		Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->Add(new HitEffect(GetPosition()));

		ReduceFuel(RockHitDecFuel);

	}

	else if (other_object->Type() == GameObjectTypes::Monster) {
		Monster* monster = static_cast<Monster*>(other_object);
		std::array<vec2, 4> points = monster->GetCollisionBoxPoints();
		normal = ComputeCollisionNormal(points, GetPosition(), monster->GetPosition());
		Engine::GetLogger().LogEvent("normal: " + std::to_string(normal.x) + ", " + std::to_string(normal.y));
		Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->Add(new MonsterHitEffect(GetPosition()));

		ReduceFuel(MonsHitDecFuel);
	}
	if (IsFuelZero()) {
		force = {};
		return;
	}

	Engine::GetLogger().LogEvent("Velocity: " + std::to_string(velocity.x) + ", " + std::to_string(velocity.y));
	Engine::Instance()->SetSlowDownFactor(slow_down_factor);
	direction = bounceBehavior->CalculateBounceDirection(velocity, normal);

	//vec2 correction = -velocity;
	//SetPosition(GetPosition() + correction);

	float speed = velocity.Length();

	slow_down += 1500.f;
	if (speed >= 5000.f) {
		speed = 5000.f - slow_down;
		//std::cout << "ship->slow_down: " << slow_down << std::endl;
	}
	else if (speed <= 800.f) {
		speed = 800.f;
	}
	else {
		speed -= slow_down;
	}

	force = direction * speed * 0.8f;
	force *= 8.0f;

	//Engine::GetLogger().LogEvent("velocity.Length(), speed : " + std::to_string(velocity.Length()) + " : " + std::to_string(speed));
}

void Ship::ReduceFuel(float value)
{
	fuel -= value;
	if (fuel < 0.0f) {
		fuel = 0.0f;
		change_state(&state_die);
	}
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
		fuelcounter += (float)dt;

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

void Ship::SetMaxFuel(float input)
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
	beat = &Engine::GetBeat();
}

void Pump::Update(double dt)
{
	GameObject::Update(dt);
	Ship* ship = Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->GetGOComponent<Ship>();
	SetPosition(ship->GetPosition());
	GetMatrix();

	if (!ship->IsFuelZero()) {
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

}

void Pump::Draw(DrawLayer drawlayer)
{
	CircleDrawCall draw_call = {
	min_pump_radius,                      // Texture to draw
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
	draw_call2.SetUniforms = [this](const GLShader* shader) {this->SetUniforms(shader); };
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
