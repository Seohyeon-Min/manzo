#include "GameOver.h"
#include "Background.h"
GameOver::GameOver()
{
}
void GameOver::Load()
{
	time = 0;

	//camera
	camera = new Cam();
	AddGSComponent(camera);

	//background
	background = new Background();
	AddGSComponent(background);
	GetGSComponent<Background>()->Add("assets/images/background/gameover.png", 0.0f);
}
void GameOver::Update(double dt)
{
	UpdateGSComponents(dt);
	GetGSComponent<Cam>()->Update(dt, {0,0}, false);


	time += dt;
	if (time > 5)
	{
		Engine::GetGameStateManager().ClearNextGameState();
		Engine::GetGameStateManager().SetNextGameState(static_cast<int>(States::Mode2));
	}
}

void GameOver::Draw()
{
	background->Draw(*GetGSComponent<Cam>());
}
void GameOver::Unload()
{
	GetGSComponent<Background>()->Unload();
	ClearGSComponents();
	Engine::Instance().UnloadTmpPtr();
	Engine::GetGameStateManager().GetGSComponent<Fish>()->ClearMoney();
}