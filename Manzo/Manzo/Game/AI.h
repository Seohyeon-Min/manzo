#pragma once

#include "../Engine/GameObject.h"
#include "GameObjectTypes.h"
#include "../Engine/MapManager.h"
#include "../Engine/AABB.h"


class BackgroundFish : public CS230::GameObject
{
public:
    BackgroundFish();
    ~BackgroundFish() = default;
    GameObjectTypes Type() override { return GameObjectTypes::BackgroundFish; }
    std::string TypeName() override { return "Background Fish"; }

    void Update(double dt);
    void Draw();

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

    ivec2 windowSize = { Engine::window_width, Engine::window_height };

    std::array<std::pair<float, float>, 3> yRanges = { std::make_pair(-800.0f, -500.0f), std::make_pair(-150.0f, 150.0f), std::make_pair(400.0f, 700.0f) };
};

static std::vector<BackgroundFish*> globalLeaders;
static std::vector<BackgroundFish*> backgroundFishList;

class Quadtree {
    static const int MAX_CAPACITY = 60;
    AABB boundary;
    std::vector<BackgroundFish*> fish;
    bool divided = false;

    Quadtree* northeast = nullptr;
    Quadtree* northwest = nullptr;
    Quadtree* southeast = nullptr;
    Quadtree* southwest = nullptr;

public:
    Quadtree(AABB boundary) : boundary(boundary) {}

    ~Quadtree() {
        delete northeast;
        delete northwest;
        delete southeast;
        delete southwest;
    }

    bool insert(BackgroundFish* fishObj) {
        if (!boundary.contains(fishObj->GetPosition())) return false;

        if (fish.size() < MAX_CAPACITY) {
            fish.push_back(fishObj);
            return true;
        }

        if (!divided) subdivide();

        return (northeast->insert(fishObj) || northwest->insert(fishObj) ||
            southeast->insert(fishObj) || southwest->insert(fishObj));
    }

    void query(const AABB& range, std::vector<BackgroundFish*>& found) const {
        if (!boundary.intersects(range)) return;

        for (auto* f : fish) {
            if (range.contains(f->GetPosition())) found.push_back(f);
        }

        if (!divided) return;

        northeast->query(range, found);
        northwest->query(range, found);
        southeast->query(range, found);
        southwest->query(range, found);
    }

    void clear() {
        fish.clear();
        if (divided) {
            northeast->clear();
            northwest->clear();
            southeast->clear();
            southwest->clear();
            delete northeast;
            delete northwest;
            delete southeast;
            delete southwest;
            divided = false;
        }
    }

private:
    void subdivide() {
        vec2 half = boundary.halfSize * 0.5f;
        vec2 center = boundary.center;

        northeast = new Quadtree(AABB{ center + vec2{half.x, -half.y}, half });
        northwest = new Quadtree(AABB{ center + vec2{-half.x, -half.y}, half });
        southeast = new Quadtree(AABB{ center + vec2{half.x, half.y}, half });
        southwest = new Quadtree(AABB{ center + vec2{-half.x, half.y}, half });

        divided = true;
    }
};