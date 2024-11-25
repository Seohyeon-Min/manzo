#include "AI.h"

BackgroundFish::BackgroundFish() : GameObject({ start_position })
{
	ivec2 windowSize = { Engine::window_width, Engine::window_height };
	start_position = { -640 ,((float)rand() / RAND_MAX) * 2.0f * windowSize.y - windowSize.y }; //outside of window
	//start_position = { -500,100 };
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

	// Boids parameters
	const float separationDistance = 50.0f;
	const float alignmentWeight = 3.0f;
	const float cohesionWeight = 0.5f;
	const float separationWeight = 0.2f;
	const float wanderWeight = 0.1f;
	const float wanderJitter = 0.5f;
	const float baseSpeed = 30.0f;
	const float followWeight = 0.5f;  // Weight for following front fish

	vec2 alignment(0.0f, 0.0f);
	vec2 cohesion(0.0f, 0.0f);
	vec2 separation(0.0f, 0.0f);
	vec2 wanderForce(0.0f, 0.0f);
	vec2 followFront(0.0f, 0.0f);  // Vector for following the front fish

	int neighborCount = 0;
	float closestFrontDistance = std::numeric_limits<float>::max();  // Track closest front fish

	// Calculate wander force
	wanderForce.x += ((float(rand()) / RAND_MAX) - 0.5f) * wanderJitter;
	wanderForce.y += ((float(rand()) / RAND_MAX) - 0.5f) * wanderJitter;
	wanderForce = wanderForce.Normalize() * wanderWeight;

	// Iterate over all fish to calculate flocking behavior
	for (auto& other : backgroundFishes) {
		if (other == this) continue;  // Ignore self

		vec2 toNeighbor = other->GetPosition() - GetPosition();
		float distance = toNeighbor.Length();

		// Only consider nearby fish as "neighbors"
		if (distance < separationDistance * 3.0f) {
			alignment += other->GetVelocity();
			cohesion += other->GetPosition();

			vec2 sub = toNeighbor / distance;
			if (distance < separationDistance) {
				separation.x -= sub.x;  // Weighted by inverse distance
				separation.y -= sub.y;
			}

			// Check if the other fish is in front along the x-axis
			if (other->GetPosition().x > GetPosition().x && distance < closestFrontDistance) {
				closestFrontDistance = distance;
				followFront = toNeighbor.Normalize();  // Normalize and store direction to front fish
			}

			neighborCount++;
		}
	}

	if (neighborCount > 0) {
		alignment = (alignment / (float)neighborCount).Normalize() * alignmentWeight;
		cohesion = ((cohesion / (float)neighborCount) - GetPosition()).Normalize() * cohesionWeight;
		separation = separation.Normalize() * separationWeight;
	}

	// Apply followFront with specified weight if a front fish is found
	followFront *= followWeight;

	// Combine boid forces, wander force, and followFront force
	vec2 boidForce = alignment + cohesion + separation + wanderForce + followFront;

	// Calculate oscillation for swimming effect
	float oscillationAmplitude = 5.0f;
	float oscillationFrequency = 1.0f;
	vec2 phaseOffset = { GetPosition().x * 10.0f, GetPosition().y * 5.0f };

	// Apply sine wave to velocity for a "swimming" effect
	float oscillatoryX = oscillationAmplitude * sin(oscillationFrequency * (float)dt + phaseOffset.x);
	float oscillatoryY = oscillationAmplitude * sin(oscillationFrequency * (float)dt + phaseOffset.y);

	// Adjust velocity and position based on combined forces and oscillatory effect
	vec2 finalVelocity = (GetVelocity() + boidForce).Normalize() * baseSpeed;
	finalVelocity.x += oscillatoryX;
	finalVelocity.y += oscillatoryY;

	SetVelocity(finalVelocity);

	// Destroy fish if it moves outside of world bounds
	if (GetPosition().x > Engine::window_width * 0.5f + 50) {
		Destroy();
	}
}

void BackgroundFish::Draw()
{
	GameObject::Draw();
}