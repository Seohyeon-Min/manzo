#include "AI.h"
#include "ScreenWrap.h"

std::vector<BackgroundFish*> BackgroundFish::globalLeaders;

BackgroundFish::BackgroundFish() : GameObject({ start_position })
{
    ivec2 windowSize = { Engine::window_width, Engine::window_height };
    start_position = { ((float)rand() / RAND_MAX) * 2.0f * windowSize.x - windowSize.x ,((float)rand() / RAND_MAX) * 2.0f * windowSize.y - windowSize.y }; // Spawn outside of window

    SetPosition(start_position);
    SetVelocity({ 30,0 });

    AddGOComponent(new CS230::Sprite("assets/images/BackgroundFish.spt", this));

    if (globalLeaders.size() < 6) {
        if (std::find(globalLeaders.begin(), globalLeaders.end(), this) == globalLeaders.end()) {
            globalLeaders.push_back(this);
        }
    }

    AddGOComponent(new ScreenWrap(*this));
}

void BackgroundFish::Update(double dt) {

    vec2 alignment(0.0f, 0.0f);
    vec2 cohesion(0.0f, 0.0f);
    vec2 separation(0.0f, 0.0f);
    vec2 wanderForce(0.0f, 0.0f);
    int neighborCount = 0;

    GameObject::Update(dt);
    elapsedTime += dt;

    static Quadtree quadtree(AABB{ {-Engine::window_width, -Engine::window_height}, {Engine::window_width, Engine::window_height} });
    quadtree.clear();

    // ����Ʈ���� ��� ����� ����
    for (auto& fish : backgroundFishes) {
        quadtree.insert(fish);
    }

    // ��ǥ ���� ����
    const float targetDistance = 30.0f;
    const float maxSeparationStrength = 15.0f;

    // �������� �̿� ������ ó��
    if (elapsedTime < 8000) {
        for (auto* leader : globalLeaders) {
            AABB searchRange = { leader->GetPosition(), vec2{50.0f, 50.0f} };
            std::vector<BackgroundFish*> neighbors;
            quadtree.query(searchRange, neighbors);

            // ���� �߽����� �̿� �����鿡 ������ �ִ� �ൿ ����
            for (auto* neighbor : neighbors) {
                if (neighbor == leader) continue;

                vec2 toNeighbor = neighbor->GetPosition() - GetPosition();
                float distance = toNeighbor.Length();

                // ��ǥ �Ÿ������� �и��� ���
                if (distance < targetDistance) {
                    vec2 pushAway = toNeighbor.Normalize() * (maxSeparationStrength / distance);
                    separation += -pushAway;  // �־������� ��
                }
                else if (distance > targetDistance) {
                    vec2 pullCloser = toNeighbor.Normalize() * ((distance - targetDistance) * 0.1f);
                    separation += pullCloser;  // ����������� ��
                }

                alignment += neighbor->GetVelocity();
                cohesion += neighbor->GetPosition();

                neighborCount++;
            }
        }

        // ���� ����� ���� ã��
        BackgroundFish* closestLeader = nullptr;
        float closestDistance = std::numeric_limits<float>::max();

        for (auto* leader : globalLeaders) {
            float distanceToLeader = (GetPosition() - leader->GetPosition()).Length();
            if (distanceToLeader < closestDistance) {
                closestLeader = leader;
                closestDistance = distanceToLeader;
            }
        }

        // ������ ã�Ҵٸ� �� ������ ����
        if (closestLeader) {
            vec2 toLeader = (closestLeader->GetPosition() - GetPosition()).Normalize() * 2.0f;
            wanderForce += toLeader;  // ������ ������ ��
        }

        if (neighborCount > 0) {
            alignment = (alignment / (float)neighborCount).Normalize() * 5.0f;
            cohesion = ((cohesion / (float)neighborCount) - GetPosition()).Normalize() * 0.5f;

            // Cohesion���� ��ǥ �Ÿ��� ���
            float cohesionStrength = 0.1f;
            cohesion *= cohesionStrength * (targetDistance / 30.0f);

            separation = separation.Normalize() * maxSeparationStrength;
        }

        // ����Ⱑ ��Ż���� �ʵ��� ������ �̵�
        wanderForce.x += ((float(rand()) / RAND_MAX) - 0.5f) * 0.2f;
        wanderForce.y += ((float(rand()) / RAND_MAX) - 0.5f) * 0.2f;

        // ��� ���� ��ħ
        vec2 boidForce = alignment * 0.3f + cohesion * 0.4f + separation * 0.35f + wanderForce * 0.01f;

        // ���� �ӵ� ����
        vec2 newVelocity = GetVelocity() + boidForce;
        newVelocity = newVelocity.Normalize() * std::min(newVelocity.Length(), 15.0f);
        SetVelocity(newVelocity);
    }
    else {
        SetVelocity({ GetVelocity().x ,0 });

        BackgroundFish* closestLeader = nullptr;
        float closestDistance = std::numeric_limits<float>::max();

        for (auto* leader : globalLeaders) {
            float distanceToLeader = (GetPosition() - leader->GetPosition()).Length();
            if (distanceToLeader < closestDistance) {
                closestLeader = leader;
                closestDistance = distanceToLeader;
            }
        }

        // ������ ã�Ҵٸ� �� ������ ����
        if (closestLeader) {
            vec2 toLeader = (closestLeader->GetPosition() - GetPosition()).Normalize() * 2.0f;
            wanderForce += toLeader;  // ������ ������ ��
        }
    }
}

void BackgroundFish::Draw()
{
    GameObject::Draw();
}
