#pragma once

#include "vec2.h"
#include "Engine.h"
#include "Component.h"
#include <vector>
#include "GameObject.h"
#include "..\Game\GameObjectTypes.h"

struct [[nodiscard]] Polygon
	{
	std::vector<vec2> vertices;
	int vertexCount;
};

class Rock : public CS230::GameObject
{
public:
	Rock(Polygon);
	GameObjectTypes Type() override { return GameObjectTypes::Reef; }
	std::string TypeName() override { return "Polygon"; }
	void Update(double dt);
	void Draw();
private:
	Polygon poly;
};

namespace CS230 {
	class Map : public CS230::Component{
	public:

		void ParseSVG(const std::string& filename);
		void AddDrawCall();
	private:
		
		std::vector<Rock> objects;
		
	};

}

