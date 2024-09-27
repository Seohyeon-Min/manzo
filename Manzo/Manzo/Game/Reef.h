#pragma once
#include "../Engine/GameObject.h"
#include "GameObjectTypes.h"

class Reef : public CS230::GameObject
{
public:
	Reef(vec2 position);
	GameObjectTypes Type() override { return GameObjectTypes::Reef; }
	std::string TypeName() override { return "Reef"; }

private:
	void Update(double dt);
	void Draw();
};