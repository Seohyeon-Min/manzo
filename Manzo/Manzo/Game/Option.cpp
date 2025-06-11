#include "Option.h"
#include <iostream>

GameOption::GameOption() : GameObject({0,0})
{
	AddGOComponent(new Sprite("assets/images/Option.spt", this));
}

void GameOption::Update(double dt)
{
	if(opened)
		GameObject::Update(dt);
}

void GameOption::Draw(DrawLayer drawlayer)
{
		GameObject::Draw();
}