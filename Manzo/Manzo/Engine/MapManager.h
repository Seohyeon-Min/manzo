#pragma once

#include "vec2.h"
#include "Engine.h"
#include "Component.h"
#include <vector>

struct [[nodiscard]] Polygon
	{
	std::vector<vec2> vertices;
	int vertexCount;
};

extern std::vector<Polygon> objects;

namespace CS230 {
	class Map : public CS230::Component{
	public:
		void ParseSVG(const std::string& filename);
		std::vector<Polygon> GetMapObject();


	private:
		
		
	};

}

