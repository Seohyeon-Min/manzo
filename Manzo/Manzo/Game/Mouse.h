#pragma once
#include "..\Engine\Particle.h"
#include "..\Engine\GameObjectManager.h"
#include "..\Engine\Component.h"


    class Mouse : public CS230::Component {
    public:
        Mouse();
        void Update(double dt) override;

        void FollowMouse(const vec2& mouse_position);
        void ExplodeOnClick(const vec2& click_position);
    };
