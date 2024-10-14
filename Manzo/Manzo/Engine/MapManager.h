#pragma once

#include "Vec2.h"
#include "Engine.h"
#include "Component.h"
#include <vector>



namespace CS230 {
	class Map : public CS230::Component{
	public:
		void ParseSVG(const std::string& filename);

	private:
		typedef struct {
			std::vector<vec2> vertices;
			int vertexCount;
		} Polygon;

		std::vector<Polygon> objects;
	};

}

