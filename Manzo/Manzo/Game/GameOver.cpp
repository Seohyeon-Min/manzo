#include "GameOver.h"

GameOver::GameOver()
{
}

void GameOver::Load()
{
	background = new Background();
	AddGSComponent(background);

	camera = new CS230::Cam();
	AddGSComponent(camera);

	GetGSComponent<Background>()->Add("assets/images/temp_back.png", 0.25f);
}

void GameOver::Update(double dt)
{
	UpdateGSComponents(dt);
	time += dt;

	if (time > 5)
	{
		Engine::GetGameStateManager().ClearNextGameState();
		Engine::GetGameStateManager().SetNextGameState(static_cast<int>(States::Mode2));
	}
}

void GameOver::Draw()
{
	GetGSComponent<Background>()->Draw(*GetGSComponent<CS230::Cam>());
}

void GameOver::Unload()
{
	GetGSComponent<Background>()->Unload();
	ClearGSComponents();
	Engine::Instance().UnloadTmpPtr();
}
