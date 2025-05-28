#include "Lerp.h"

vec2 Lerp(const vec2& a, const vec2& b, float t) {
    return a + (b - a) * t;
}