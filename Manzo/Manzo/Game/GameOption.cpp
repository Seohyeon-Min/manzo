#include "GameOption.h"

GameOption::GameOption(vec2 pos) : GameObject(pos)
{
    AddGOComponent(new Sprite("assets/images/Option.spt", this));
}

void GameOption::Update(double dt)
{
    if (opened)
        GameObject::Update(dt);
}

void GameOption::Draw(DrawLayer drawlayer)
{
    if(opened)
        GameObject::Draw(DrawLayer::DrawFirst);
}
