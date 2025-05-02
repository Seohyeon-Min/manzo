#pragma once
#include "../Engine/GameObject.h"
#include "GameObjectTypes.h"
#include "../Engine/Procedual.h"
//heheh

class BossBullet : public GameObject
{
public:
    ProceduralChain bullet_chain;
    BossBullet(vec2 Boss_position, float lifetime);
    GameObjectTypes Type() override { return GameObjectTypes::BossBullet; }
    std::string TypeName() override { return "BossBullet"; }
    void Update(double dt) override;
    void Draw(DrawLayer drawlayer = DrawLayer::Draw) override;
    void Move(double dt);
    bool CanCollideWith(GameObjectTypes) override;
    void ResolveCollision(GameObject* other_object) override;

private:
    vec2 bullet_position;
    vec2 velocity;
    vec2 targetPosition;
    double lifetime;
    double speed;
    double timeElapsed;
    vec2 toPlayer;
    float distanceToPlayer;
};