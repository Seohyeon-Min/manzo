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


namespace CS230 {
	class Map : public CS230::Component{
	public:
		std::vector<Polygon> objects;

		void ParseSVG(const std::string& filename);
		static std::vector<Polygon>& getObject();
	private:
		
		
	};

}

