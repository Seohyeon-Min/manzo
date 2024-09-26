#pragma once
#include "GameObject.h"
#include "GameObjectManager.h"
#include "math.h"

namespace CS230 {
	class Light {
	public:
		static void AddShadowPoint(vec2 position);
		static void AddshadowObject();
		static void Update(vec2 playerposition, vec2 shadowpoint, vec2 Collinearpoint, vec2 nextCollinearpoint);
		static void Draw(float light_size);
		static void AddLightPosition(vec2 position);


	private:

	};

}