#include "AI.h"

BackgroundFish::BackgroundFish() : GameObject({ start_position })
{
	ivec2 windowSize = { Engine::window_width, Engine::window_height };
	start_position = { ((float)rand() / RAND_MAX) * 2.0f * windowSize.x - windowSize.x ,((float)rand() / RAND_MAX) * 2.0f * windowSize.y - windowSize.y }; //outside of window

	SetPosition(start_position);
	SetVelocity({ 30,0 });

	AddGOComponent(new CS230::Sprite("assets/images/BackgroundFish.spt", this));
}

float BackgroundFish::PointToSegmentDistance(const vec2& point, const vec2& segmentStart, const vec2& segmentEnd) {
	vec2 segmentDir = segmentEnd - segmentStart;
	vec2 toPoint = point - segmentStart;

	float segmentLengthSquared = magnitude_squared(segmentDir);
	if (segmentLengthSquared == 0.0f) {
		return toPoint.Length();  // 선분의 길이가 0이면 점과 선분의 거리는 점과 점 사이의 거리
	}

	float projection = dot(toPoint, segmentDir) / segmentLengthSquared;
	projection = std::clamp(projection, 0.0f, 1.0f);  // [0, 1] 범위로 클램프

	vec2 closestPoint = segmentStart + projection * segmentDir;
	return (point - closestPoint).Length();
}

void BackgroundFish::Update(double dt) {
    GameObject::Update(dt);

    static Quadtree quadtree(AABB{ {0.0f, 0.0f}, {Engine::window_width / 2.0f, Engine::window_height / 2.0f} });
    quadtree.clear();

    // Insert all fish into the Quadtree
    for (auto& fish : backgroundFishes) {
        quadtree.insert(fish);
    }

    // Define the search range
    AABB searchRange = { GetPosition(), vec2{150.0f, 150.0f} };
    std::vector<BackgroundFish*> neighbors;
    quadtree.query(searchRange, neighbors);

    vec2 alignment(0.0f, 0.0f);
    vec2 cohesion(0.0f, 0.0f);
    vec2 separation(0.0f, 0.0f);
    vec2 wanderForce(0.0f, 0.0f);

    int neighborCount = 0;

    for (auto* neighbor : neighbors) {
        if (neighbor == this) continue;

        vec2 toNeighbor = neighbor->GetPosition() - GetPosition();
        float distance = toNeighbor.Length();

        alignment += neighbor->GetVelocity();
        cohesion += neighbor->GetPosition();

        if (distance < 50.0f) {
            separation += toNeighbor / -distance;
        }

        neighborCount++;
    }

    if (neighborCount > 0) {
        alignment = (alignment / (float)neighborCount).Normalize() * 3.0f;
        cohesion = ((cohesion / (float)neighborCount) - GetPosition()).Normalize() * 0.5f;
        separation = separation.Normalize() * 0.2f;
    }

    wanderForce.x += ((float(rand()) / RAND_MAX) - 0.5f) * 0.5f;
    wanderForce.y += ((float(rand()) / RAND_MAX) - 0.5f) * 0.5f;

    vec2 boidForce = alignment + cohesion + separation + wanderForce;

    // Update velocity and position
    SetVelocity((GetVelocity() + boidForce).Normalize() * 30.0f);

    // Screen boundary detection and flip logic
    vec2 pos = GetPosition();
    vec2 vel = GetVelocity();

    // Check horizontal boundaries
    if (pos.x <= -Engine::window_width || pos.x >= Engine::window_width) {
        vel.x = -vel.x;  // Flip horizontal direction
    }

    SetVelocity(vel);
}

void BackgroundFish::Draw()
{
	GameObject::Draw();
}