#pragma once
#include "vec2.h"
#include <vector>

struct [[nodiscard]] Polygon
{
	std::vector<vec2> vertices;
	int vertexCount;
	int polycount;
	std::string polyindex;
};


struct [[nodiscard]] EntryData {
	float Isnotelong;
	vec2 position;
	float delay;


};