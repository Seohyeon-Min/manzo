#pragma once
#include "..\Engine\GameObject.h"
#include "GameObjectTypes.h"

class Player : public GameObject {
public:
    Player(vec2 start_position);
    GameObjectTypes Type() override { return GameObjectTypes::Player; }
    std::string TypeName() override { return "Cat"; }
    void Update(double dt) override;
    void Draw(DrawLayer drawlayer = DrawLayer::Draw) override;

private:
    float speed = 500.f;

    class Player_Idle : public State {
    public:
        virtual void Enter(GameObject* object) override;
        virtual void Update(GameObject* object, double dt) override;
        virtual void CheckExit(GameObject* object) override;
        std::string GetName() override { return "Idle"; }
    };

    class Player_Walking : public State {
    public:
        virtual void Enter(GameObject* object) override;
        virtual void Update(GameObject* object, double dt) override;
        virtual void CheckExit(GameObject* object) override;
        std::string GetName() override { return "Walking"; }
    };

    Player_Idle state_idle;
    Player_Walking state_walking;

    enum class Animations {
        Idle,
        Walking,
    };

};
