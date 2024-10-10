#pragma once

#include "Vec2.h"
#include "Engine.h"
#include <vector>
#include <Eigen/Dense>


using Eigen::Vector2f;

namespace CS230 {
	class Map {
	public:

		void parseSVG(const std::string& filename);


	private:
		

	};

}

typedef struct {
	std::vector<Vector2f> vertices;
	int vertexCount;
} Polygon;

std::vector<Polygon> objects;	