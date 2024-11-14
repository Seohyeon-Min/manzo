#include "Clear.h"

Clear::Clear()
{
}

void Clear::Load()
{
	//camera
	camera = new CS230::Cam();
	AddGSComponent(camera);

	//background
	background = new Background();
	AddGSComponent(background);
	GetGSComponent<Background>()->Add("assets/images/background/clear.png", 0.25f); //need change asset
}

void Clear::Update(double dt)
{
	UpdateGSComponents(dt);
	GetGSComponent<CS230::Cam>()->Update(dt, { 0,0 }, false);
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
	Engine::GetGameStateManager().GetGSComponent<Fish>()->ClearMoney();
}