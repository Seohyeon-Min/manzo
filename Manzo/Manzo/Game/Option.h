#pragma once

#include "../Engine/GameObject.h"
#include "GameObjectTypes.h"

class GameOption : public GameObject
{
public:
	GameOption();
	GameObjectTypes Type() override { return GameObjectTypes::UI; }
	std::string TypeName() override { return "Option Window UI"; }

	void Update(double dt) override;
	void Draw(DrawLayer drawlayer = DrawLayer::Draw) override;

	void SetOpen(bool open) { opened = open; }
	bool IsOpened() { return opened; }

private:
	bool opened;
};