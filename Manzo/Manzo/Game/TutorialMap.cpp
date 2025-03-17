#include "TutorialMap.h"
#include "BeatSystem.h"


Tutorial::Tutorial()
{
}

void Tutorial::Load()
{
	AddGSComponent(new GameObjectManager());

	beat_system = new Beat();
	AddGSComponent(beat_system);


}

void Tutorial::Update(double dt)
{
}

void Tutorial::FixedUpdate(double dt)
{
}

void Tutorial::Unload()
{
}

void Tutorial::Draw()
{
}
