#pragma once
#include "vec2.h"
#include "Rect.h"
#include <vector>

struct [[nodiscard]] Polygon
{
	std::vector<vec2> vertices;
	int vertexCount = 3;
	int polycount = 0;
	std::string polyindex = "";

	vec2 FindCenter() {			// calculate center of polygon
		vec2 center = { 0, 0 };
		for (vec2& vertice : vertices) {
			center.x += vertice.x;
			center.y += vertice.y;
		}
		center.x /= vertexCount;
		center.y /= vertexCount;
		return center;
	}

	Math::rect FindBoundary() {
		if (vertices.empty()) {
			return Math::rect{};
		}
		vec2 minPoint = vertices[0];
		vec2 maxPoint = vertices[0];
		for (const auto& vertex : vertices) {
			minPoint.x = std::min(minPoint.x, vertex.x);
			minPoint.y = std::min(minPoint.y, vertex.y);
			maxPoint.x = std::max(maxPoint.x, vertex.x);
			maxPoint.y = std::max(maxPoint.y, vertex.y);
		}
		return Math::rect{ minPoint, maxPoint };
	}
};


struct [[nodiscard]] EntryData {
	float attacktype;
	vec2 position;
	float delay;


};