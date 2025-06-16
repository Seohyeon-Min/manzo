#pragma once
#include "vec2.h"
#include <algorithm>
#include <mat3.h>

#ifndef M_PI
#define M_PI 3.14
#endif

/*
* 
* We have

constexpr float dot(vec2 a, vec2 b) noexcept

constexpr float cross(vec2 a, vec2 b) noexcept

constexpr vec2 perpendicular_to(vec2 a) noexcept

constexpr float magnitude_squared(vec2 a) noexcept

Nomalize

*/

vec2 ClosestPoint(vec2 point, vec2 a, vec2 b);
vec2 Lerp(const vec2& start, const vec2& end, float t);
float Lerp(float a, float b, float t);

mat3 Inverse(mat3& m);
