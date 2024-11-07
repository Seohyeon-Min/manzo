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
	bool MatchIndex();
	Polygon GetPoly() { return poly; }
private:
	Polygon poly;
};


class RockGroup : public CS230::GameObject //group of polys
{
public:
	RockGroup(const std::string& index);
	~RockGroup() {};
	GameObjectTypes Type() override { return GameObjectTypes::Reef; }
	std::string TypeName() override { return "Rock Group"; }
	void Update(double dt);
	void Draw();
	std::vector<Polygon> GetRocks() { return rocks; }
	bool MatchIndex();
	vec2 FindCenter();
	std::string GetIndex() { return index; }
private:
	std::vector<Polygon> rocks;	//one group
	std::string index;
};


namespace CS230 {
	class Map : public CS230::Component {
	public:

		void ParseSVG(const std::string& filename);
		void AddDrawCall();
	private:

		std::vector<Rock> objects;
		std::vector<RockGroup> rock_groups;		//vector for groups

	};

}
