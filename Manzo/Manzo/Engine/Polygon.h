
#pragma once
#include "vec2.h"
#include <vector>

struct [[nodiscard]] Polygon
{
	std::vector<vec2> vertices;
	int vertexCount;
};