#pragma once

#include "../Engine/GameObject.h"
#include "GameObjectTypes.h"
#include "../Engine/MapManager.h"
#include "WindowState.h"

class BackgroundFish : public GameObject
{
public:
    BackgroundFish();
    ~BackgroundFish() = default;
    GameObjectTypes Type() override { return GameObjectTypes::BackgroundFish; }
    std::string TypeName() override { return "Background Fish"; }

    void Update(double dt) override;
    void Draw(DrawLayer drawlayer = DrawLayer::Draw) override;

    static float a, b, c, d;

    class State_Leader : public State {
    public:
        virtual void Enter(GameObject* object) override;
        virtual void Update(GameObject* object, double dt) override;
        virtual void CheckExit(GameObject* object) override;
        std::string GetName() override { return "State_Leader"; }
    };

    class State_Nonleader : public State {
    public:
        virtual void Enter(GameObject* object) override;
        virtual void Update(GameObject* object, double dt) override;
        virtual void CheckExit(GameObject* object) override;
        std::string GetName() override { return "State_Nonleader"; }
    };

    State_Leader state_leader;
    State_Nonleader state_nonleader;

private:

    vec2 alignment{ 0.f,0.f };
    vec2 cohesion{ 0.f,0.f };
    vec2 separation{ 0.f,0.f };
    vec2 wanderForce{ 0.f,0.f };
    vec2 boidForce;

    vec2 start_position;

    float minDistance = 20.0f;
    float closestDistance = std::numeric_limits<float>::max();

    ivec2 windowSize = { (int)Engine::window_width, (int)Engine::window_height };

    std::array<std::pair<float, float>, 3> yRanges = { std::make_pair(-800.0f, -500.0f), std::make_pair(-150.0f, 150.0f), std::make_pair(400.0f, 700.0f) };
};

static std::vector<BackgroundFish*> globalLeaders;
static std::vector<BackgroundFish*> backgroundFishList;

