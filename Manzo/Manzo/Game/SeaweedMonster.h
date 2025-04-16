#pragma once
#pragma once
#include "../Engine/GameObject.h"

class SeaweedMonster : public GameObject {
	SeaweedMonster(vec2 pos);
	GameObjectTypes Type() override { return GameObjectTypes::Monster; }
	std::string TypeName() override { return "Monster"; }
	void Update(double dt)override;
	void Draw(DrawLayer drawlayer = DrawLayer::Draw)override;
};