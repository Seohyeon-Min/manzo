#include "AI.h"
#include "ScreenWrap.h"

std::vector<BackgroundFish*> BackgroundFish::globalLeaders;
float BackgroundFish::a, BackgroundFish::b, BackgroundFish::c, BackgroundFish::d = 0.f;

BackgroundFish::BackgroundFish() : GameObject({ start_position })
{
    ivec2 windowSize = { Engine::window_width, Engine::window_height };
    int rangeIndex = rand() % yRanges.size();
    float yMin = yRanges[rangeIndex].first;
    float yMax = yRanges[rangeIndex].second;

    start_position = {
        ((float)rand() / RAND_MAX) * 2.0f * windowSize.x - windowSize.x,
        yMin + ((float)rand() / RAND_MAX) * (yMax - yMin)
    };


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

    static Quadtree quadtree(AABB{ {-Engine::window_width, -Engine::window_height}, {Engine::window_width, Engine::window_height} });
    quadtree.clear();

    // ����Ʈ���� ��� ����� ����
    for (auto& fish : backgroundFishes) {
        quadtree.insert(fish);
    }

    // �������� �̿� ������ ó��
    for (auto* leader : globalLeaders) {
        AABB searchRange = { leader->GetPosition(), vec2{50.0f, 50.0f} };
        std::vector<BackgroundFish*> neighbors;
        quadtree.query(searchRange, neighbors);

        // ���� �߽����� �̿� �����鿡 ������ �ִ� �ൿ ����
        for (auto* neighbor : neighbors) {
            if (neighbor == leader) continue;

            vec2 toLeader = leader->GetPosition() - neighbor->GetPosition();
            float distance = toLeader.Length();

            if (distance < 50.0f) {
                vec2 influence = toLeader.Normalize() * 2.0f;
                neighbor->SetVelocity(neighbor->GetVelocity() + influence);
            }

            alignment += neighbor->GetVelocity();
            cohesion += neighbor->GetPosition();

            // Separation: ����� ���� �ּ� �Ÿ��� 20���� ���� ��� �и��� ����
            if (distance < minDistance) {
                vec2 pushAway = (GetPosition() - neighbor->GetPosition()).Normalize();
                separation += pushAway * (separationStrength / distance);
            }
            else if (distance < 100.0f) {
                separation += toLeader / -distance;
            }

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
        vec2 toLeader = (closestLeader->GetPosition() - GetPosition()).Normalize();
        wanderForce += toLeader;  // ������ ������ ��
    }

    if (neighborCount > 0) {
        alignment = (alignment / (float)neighborCount).Normalize();
        cohesion = ((cohesion / (float)neighborCount) - GetPosition()).Normalize();
        separation = separation.Normalize();
    }

    // ����Ⱑ ��Ż���� �ʵ��� ������ �̵�
    wanderForce.x += ((float(rand()) / RAND_MAX) - 0.5f) * 0.5f;
    wanderForce.y += ((float(rand()) / RAND_MAX) - 0.5f) * 0.5f;

    // ��� ���� ��ħ
    vec2 boidForce = alignment * a + cohesion * b + separation * c + wanderForce * d;

    // Follower�� ������ �����ϴ� ����
    vec2 newVelocity = GetVelocity() + boidForce;
    newVelocity.x = newVelocity.Normalize().x * std::min(newVelocity.Length(), 15.0f);  // �ִ� �ӵ� ����
    newVelocity.y = newVelocity.Normalize().y * std::min(newVelocity.Length(), 15.0f);  // �ִ� �ӵ� ����
    SetVelocity(newVelocity);

    if (GetVelocity().x < 0)
    {
        SetFlipX(true);
    }
    else
    {
        SetFlipX(false);
    }

    /*if (pos.x <= -Engine::window_width || pos.x >= Engine::window_width) vel.x = -vel.x;
    if (pos.y <= -Engine::window_height || pos.y >= Engine::window_height) vel.y = -vel.y;*/
}

void BackgroundFish::Draw()
{
    GameObject::Draw();
}
