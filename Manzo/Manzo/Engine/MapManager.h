#pragma once

#include "vec2.h"
#include "Component.h"
#include "GameObject.h"
#include "..\Game\GameObjectTypes.h"
#include "Polygon.h"

#include <vector>

class Rock : public CS230::GameObject
{
public:
	Rock(Polygon poly);
	~Rock(){};
	GameObjectTypes Type() override { return GameObjectTypes::Reef; }
	std::string TypeName() override { return "Polygon"; }
	void Update(double dt);
	void Draw();
	void MatchIndex(Polygon poly);
private:
	Polygon poly;
};

namespace CS230 {
	class Map : public CS230::Component {
	public:

		void ParseSVG(const std::string& filename);
		void AddDrawCall();
	private:

		std::vector<Rock> objects;

	};

}
