#include "GameOver.h"
#include "Background.h"
GameOver::GameOver()
{
}
void GameOver::Load()
{
	time = 0;

	//camera
	camera = new CS230::Cam();
	AddGSComponent(camera);

	//background
	background = new Background();
	AddGSComponent(background);
	GetGSComponent<Background>()->Add("assets/images/background/gameover.png", 0.0f);
}
void GameOver::Update(double dt)
{
	UpdateGSComponents(dt);
	GetGSComponent<CS230::Cam>()->Update(dt, {0,0}, false);


	time += dt;
	if (time > 5)
	{
		Engine::GetGameStateManager().ClearNextGameState();
		Engine::GetGameStateManager().SetNextGameState(static_cast<int>(States::Mode2));
	}
}

void GameOver::Draw()
{
	background->Draw(*GetGSComponent<CS230::Cam>());
}
void GameOver::Unload()
{
	GetGSComponent<Background>()->Unload();
	ClearGSComponents();
	Engine::Instance().UnloadTmpPtr();
}