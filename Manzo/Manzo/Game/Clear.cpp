#include "Clear.h"

Clear::Clear()
{
}

void Clear::Load()
{
	background = new Background();
	AddGSComponent(background);
	camera = new CS230::Cam();
	AddGSComponent(camera);
	GetGSComponent<Background>()->Add("assets/images/Digipen.png", 0.25f); //need change asset
}

void Clear::Update(double dt)
{
	UpdateGSComponents(dt);
	time += dt;
	if (time > 5)
	{
		Engine::GetGameStateManager().ClearNextGameState();
		Engine::GetGameStateManager().SetNextGameState(static_cast<int>(States::Mode2));
	}
}

void Clear::Draw()
{
	GetGSComponent<Background>()->Draw(*GetGSComponent<CS230::Cam>());
}

void Clear::Unload()
{
	GetGSComponent<Background>()->Unload();
	ClearGSComponents();
	Engine::Instance().UnloadTmpPtr();
}