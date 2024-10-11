#pragma once

#include "Vec2.h"
#include "Engine.h"
#include "Component.h"
#include <vector>
#include <Eigen/Dense>


using Eigen::Vector2f;

namespace CS230 {
	class Map : public CS230::Component{
	public:
		void ParseSVG(const std::string& filename);

	private:
		typedef struct {
			std::vector<Vector2f> vertices;
			int vertexCount;
		} Polygon;

		std::vector<Polygon> objects;
	};

}

