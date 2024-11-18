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
	const Polygon& GetPolygon() { return poly; }
private:
	Polygon poly;
};


class RockGroup : public CS230::GameObject //group of polys
{
public:
	RockGroup(const std::string& index);
	~RockGroup() {
		rocks.clear();
	}
	GameObjectTypes Type() override { return GameObjectTypes::Reef; }
	std::string TypeName() override { return "Rock Group"; }
	void Update(double dt);
	void Draw();

	void AddRock(Polygon poly) { rocks.push_back(poly); }
	std::vector<Polygon> GetRocks() { return rocks; }
	bool MatchIndex();
	vec2 FindCenter();
	std::string GetIndex() { return index; }
	
private:
	mat3 matrix;
	std::vector<Polygon> rocks;	//one group
	std::string index = "";
};


namespace CS230 {
	class Map : public CS230::Component {
	public:
		~Map() {
			objects.clear();
			rock_groups.clear();
		}
		void ParseSVG(const std::string& filename);
		void AddDrawCall();

		std::vector<Rock> GetRock()
		{
			return objects;
		}

	private:

		std::vector<Rock> objects;
		std::vector<RockGroup*> rock_groups;		//vector for groups
		//std::list<RockGroup*> rock_groups;		//vector for groups
		//std::vector<std::shared_ptr<RockGroup>> rock_groups;

	};
}
