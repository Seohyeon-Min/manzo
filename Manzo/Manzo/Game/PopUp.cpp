#include "PopUp.h"
#include "PopUpMovement.h"

PopUp::PopUp(vec2 position, const std::filesystem::path& sprite_file, bool moving) : GameObject(position), moving(moving)
{
	AddGOComponent(new Sprite(sprite_file, this));

	endPos = position;

	if (moving)
	{
		startPos = { position.x - GetGOComponent<Sprite>()->GetFrameSize().x, position.y };
		
		AddGOComponent(new PopUpMovement(this, startPos, endPos));

		SetPosition(startPos);
	}
}

void PopUp::Update(double dt)
{
	if (pop)
	{
		GameObject::Update(dt);
	}
}

void PopUp::Draw(DrawLayer drawlayer)
{
	if (pop) GameObject::Draw();
}
