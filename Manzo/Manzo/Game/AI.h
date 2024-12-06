#pragma once

#include "../Engine/GameObject.h"
#include "GameObjectTypes.h"
#include "../Engine/MapManager.h"
#include "../Engine/AABB.h"

static double elapsedTime = 0.0;

class BackgroundFish : public CS230::GameObject
{
public:
	BackgroundFish();
	~BackgroundFish() { backgroundFishes.clear(); }
	GameObjectTypes Type() override { return GameObjectTypes::BackgroundFish; }
	std::string TypeName() override { return "Background Fish"; }

	void Update(double dt);
	void Draw();

	void AddBackgroundFishes(BackgroundFish* obj)
	{
		backgroundFishes.push_back(obj);
	}

private:
	vec2 start_position;
	std::list<BackgroundFish*> backgroundFishes;
    static std::vector<BackgroundFish*> globalLeaders;
	vec2 direction = { 0,0 };

	vec2 GetPerpendicular(vec2 v) {
		return { -v.y, v.x };
	}

    float minDistance = 20.0f; 
    float separationStrength = 20.0f;

    ivec2 windowSize = { Engine::window_width, Engine::window_height };
};

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