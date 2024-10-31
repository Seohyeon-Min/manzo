#pragma once
#include "Component.h"
#include "vec2.h"
#include "../Game/Ship.h"
#include <memory>

class UI:public CS230::Component {
public:
    virtual void AddDrawCall() = 0;
    struct Interface {
        GLTexture* texture;
        vec2 position;
        double scale;
    };
}; 

class fuelUI :public UI {
public:
    fuelUI();
    fuelUI(Ship* ship);
    void Update([[maybe_unused]] double dt) override;
    void AddDrawCall() override;
private:
    Ship* ship;
};