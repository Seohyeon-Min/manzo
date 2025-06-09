#include "MathUtils.h"

vec2 ClosestPoint(vec2 point, vec2 a, vec2 b) {
	vec2 ab = b - a;
	vec2 ap = point - a;

	float t = dot(ap, ab) / ab.LengthSquared();
	t = std::clamp(t, 0.0f, 1.0f);

	return a + ab * t;
}

vec2 Lerp(const vec2& a, const vec2& b, float t) {
	return a * (1.0f - t) + b * t;
}