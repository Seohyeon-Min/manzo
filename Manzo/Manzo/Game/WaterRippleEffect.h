#pragma once
#include "..\Engine\GameObject.h"
#include "GameObjectTypes.h"
#include "../Engine/TextureManager.h"

class WaterRipple : public GameObject {
public:
    WaterRipple();
    GameObjectTypes Type() override { return GameObjectTypes::Fish; }
    std::string TypeName() override { return "WaterRippleEffect"; }
    void Update(double dt) override;
    void Draw(DrawLayer drawlayer = DrawLayer::Draw) override;
private:
    void SetUni(const GLShader* shader);
    GLTexture* water;
    bool playing = false;
};