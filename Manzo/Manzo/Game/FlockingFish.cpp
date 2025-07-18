#include "FlockingFish.h"
#include "ScreenWrap.h"
#include "FishEcosystem.h"
#include "../Engine/GameObjectManager.h"
#include "GameOption.h"

float BackgroundFish::a = 0.2957f, BackgroundFish::b = 0.0f, BackgroundFish::c = 3.6f, BackgroundFish::d = 0.0174f;

BackgroundFish::BackgroundFish() : GameObject({ start_position })
{
	int rangeIndex = rand() % yRanges.size();
	float yMin = yRanges[rangeIndex].first;
	float yMax = yRanges[rangeIndex].second;

	start_position = {
		((float)rand() / RAND_MAX) * 2.0f * windowSize.x - windowSize.x,
		yMin + ((float)rand() / RAND_MAX) * (yMax - yMin)
	};


	SetPosition(start_position);
	SetVelocity({ 30,0 });

    AddGOComponent(new Sprite("assets/images/fish/BackgroundFish.spt", this));

	AddGOComponent(new ScreenWrap(*this));
}

void BackgroundFish::Update(double dt)
{
	if(!Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->GetGOComponent<GameOption>()->isOpened())
	{
		GameObject::Update(dt);

		SetFlipX(GetVelocity().x <= 0);
	}
	else
	{
		SetVelocity({ 0,0 });
	}
}

void BackgroundFish::Draw(DrawLayer drawlayer)
{
    DrawCall draw_call = {
        GetGOComponent<Sprite>()->GetTexture(),                       // Texture to draw
        &GetMatrix(),                          // Transformation matrix
        Engine::GetShaderManager().GetDefaultShader()
    };

	draw_call.sorting_layer = drawlayer;
	draw_call.settings.do_blending = true;
    GameObject::Draw(draw_call);
}

void BackgroundFish::State_Leader::Enter(GameObject* object)
{
	BackgroundFish* bgf = static_cast<BackgroundFish*>(object);
	globalLeaders.push_back(bgf);
}

void BackgroundFish::State_Leader::Update(GameObject* object, double dt)
{
	BackgroundFish* bgf = static_cast<BackgroundFish*>(object);


	bgf->boidForce = bgf->alignment * a + bgf->cohesion * b + bgf->separation * c + bgf->wanderForce * d;
	vec2 newVelocity = bgf->GetVelocity().Normalize() + bgf->boidForce;
	newVelocity = newVelocity.Normalize() * std::min(newVelocity.Length(), 15.0f);
	bgf->SetVelocity(newVelocity);
}

void BackgroundFish::State_Leader::CheckExit(GameObject* object)
{
}

void BackgroundFish::State_Nonleader::Enter(GameObject* object)
{
	BackgroundFish* bgf = static_cast<BackgroundFish*>(object);
	backgroundFishList.push_back(bgf);
}

void BackgroundFish::State_Nonleader::Update(GameObject* object, double dt)
{
	BackgroundFish* closestLeader = nullptr;
	BackgroundFish* bgf = static_cast<BackgroundFish*>(object);

	if (globalLeaders.size() > 0)
	{
		for (auto* leader : globalLeaders) {
			float distanceToLeader = (bgf->GetPosition() - leader->GetPosition()).Length();
			if (distanceToLeader < bgf->closestDistance) {
				closestLeader = leader;
				bgf->closestDistance = distanceToLeader;
			}
		}
	}

	if (closestLeader)
	{
		vec2 toLeader = (closestLeader->GetPosition() - bgf->GetPosition());
		float distance = toLeader.Length();
		bgf->wanderForce += toLeader;

		bgf->alignment += bgf->GetVelocity();
		bgf->cohesion += bgf->GetPosition();

		if (distance < bgf->minDistance) {
			vec2 pushAway = (bgf->GetPosition() - bgf->GetPosition());
			bgf->separation += pushAway * (bgf->minDistance / distance);
		}
		else if (distance < 100.0f) {
			bgf->separation += toLeader / -distance;
		}
	}

	bgf->boidForce = bgf->alignment * a + bgf->cohesion * b + bgf->separation * c + bgf->wanderForce * d;
	vec2 newVelocity = bgf->GetVelocity().Normalize() + bgf->boidForce;
	newVelocity = newVelocity.Normalize() * std::min(newVelocity.Length(), 15.0f);
	bgf->SetVelocity(newVelocity);
}

void BackgroundFish::State_Nonleader::CheckExit(GameObject* object)
{
}
