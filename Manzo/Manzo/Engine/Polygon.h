#pragma once
#include "vec2.h"
#include <vector>

struct [[nodiscard]] Polygon
{
	std::vector<vec2> vertices;
	int vertexCount;
	int polycount;
	std::string polyindex;

	vec2 FindCenter() {
		vec2 center;
		for (vec2 vertice : vertices) {
			center.x += vertice.x;
			center.y += vertice.y;
		}
		center.x /= vertexCount;
		center.y /= vertexCount;

		return center;

	}
};