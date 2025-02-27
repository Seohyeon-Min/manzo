#pragma once
#include "vec2.h"
#include "..\Engine\Component.h"

class BounceBehavior: public Component {
public:
    virtual ~BounceBehavior() = default;
    virtual vec2 CalculateBounceDirection(const vec2& incomingVelocity, const vec2& collisionNormal) const = 0;
};

class DefaultBounceBehavior : public BounceBehavior {
public:
    virtual vec2 CalculateBounceDirection(const vec2& incomingVelocity, const vec2& collisionNormal) const override {
        float dot = incomingVelocity.x * collisionNormal.x + incomingVelocity.y * collisionNormal.y;
        vec2 reflection = {
            incomingVelocity.x - 2 * dot * collisionNormal.x,
            incomingVelocity.y - 2 * dot * collisionNormal.y
        };
        float len = reflection.Length();
        return (len > 0.0f) ? reflection.Normalize() : vec2{ 1.0f, 0.0f };
    }
};