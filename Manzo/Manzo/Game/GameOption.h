#pragma once

#include "..\Engine\GameObject.h"
#include "GameObjectTypes.h"

class GameOption : public GameObject {
public:
    GameOption(vec2 pos);

    GameObjectTypes Type() override { return GameObjectTypes::UI; }
    std::string TypeName() override { return "Game Option"; }

    void Update(double dt) override;
    void Draw(DrawLayer drawlayer = DrawLayer::Draw) override;

    void SetOpen(bool opened_) { opened = opened_; }
    bool isOpened() { return opened; }

private:
    bool opened = false;
    DrawLayer layer;
};