#include "IconManager.h"
#include "..\Engine\Engine.h"
#include "..\Engine\\GameObjectManager.h"

void IconManager::Add(Icon* icon)
{
	icons.push_back(icon);
	Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->Add(icon);
}

void IconManager::UpdateAll(double dt)
{
	if (!icons.empty())
	{
		for (auto& icon : icons)
		{
			icon->Update(dt);
		}
	}
}

void IconManager::DrawAll()
{
	if (!icons.empty())
	{
		for (auto& icon : icons)
		{
			icon->Draw();
		}
	}
}

void IconManager::Unload()
{
	icons.clear();
}