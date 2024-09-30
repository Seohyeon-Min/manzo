#pragma once
#include "GameObject.h"
#include "GameObjectManager.h"
#include "math.h"

namespace CS230 {
	class Light {
	public:
		static void AddShadowPoint(Math::vec2 position);
		static void AddshadowObject();
		static void Update(Math::vec2 playerposition, Math::vec2 shadowpoint, Math::vec2 Collinearpoint, Math::vec2 nextCollinearpoint);
		static void Draw(float light_size);
		static void AddLightPosition(Math::vec2 position);


	private:

	};

}