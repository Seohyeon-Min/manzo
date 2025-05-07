#pragma once

#include "../Engine/Component.h"
#include "../Engine/GameObject.h"
#include "../Engine/Icon.h"
#include "PopUp.h"


class PopUpMovement : public Component
{
public:
	PopUpMovement(GameObject* obj, vec2 start, vec2 end) : startPos(start), endPos(end)
	{
		object = obj;

		if (obj->Type() == GameObjectTypes::Icon)
		{
			elapsed = -0.45f;
		}
	}

	~PopUpMovement() = default;

    void Update(double dt) override
    {
		elapsed += static_cast<float>(dt);

		float t = std::min(elapsed / duration, 1.0f);

		float eased = 1 - (1 - t) * (1 - t);

		vec2 newPos = startPos + (endPos - startPos) * eased;
		if(object) object->SetPosition(newPos);
		if (object_follower) object_follower->SetPosition(newPos);

		if (t >= 1.0f)
		{
			moving = false;
		}
    }

private:
	GameObject* object;
	GameObject* object_follower;

	vec2 startPos;
	vec2 endPos;

	bool pop = false;
	bool moving = false;

	float duration = 0.5f;
	float elapsed = 0.0f;
};