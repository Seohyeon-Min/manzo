#pragma once

#include "vec2.h"
#include "Component.h"
#include "GameObject.h"
#include "..\Game\GameObjectTypes.h"
#include "Polygon.h"

#include <vector>
class RockGroup;

class Rock : public CS230::GameObject
{
public:
	Rock(Polygon poly);
	~Rock(){};
	GameObjectTypes Type() override { return GameObjectTypes::Reef; }
	std::string TypeName() override { return "Polygon"; }
	void Update(double dt);
	void Draw();

	//position
	void SetCenter();

	//polygon
	const Polygon& GetPolygon() { return poly; }
	
	//group
	void SetRockGroup(RockGroup* rockgroup) { this->rockgroup = rockgroup; }
	RockGroup* GetRockGroup() { return rockgroup; }
private:
	RockGroup* rockgroup;
	Polygon poly;
};


class RockGroup : public CS230::GameObject //group of polys
{
public:
	RockGroup(const std::string& index);
	~RockGroup() {
		rocks.clear();
	}
	GameObjectTypes Type() override { return GameObjectTypes::ReefBoundary; }
	std::string TypeName() override { return "Rock Group"; }
	bool CanCollideWith(GameObjectTypes) override;
	void Update(double dt);
	void Draw();
	void ResolveCollision(GameObject* other_object);

	void AddRock(Rock* rock) { rocks.push_back(rock); }
	std::vector<Rock*> GetRocks() { return rocks; }
	bool MatchIndex();
	vec2 FindCenterRect();
	vec2 FindCenterPoly();
	std::string GetIndex() { return index; }

	// Points
	std::vector<vec2> GetPoints() { return points; }
	void RemoveDuplicatePoints(std::vector<vec2>& points, float tolerance);
	void SetPoints();
	
private:
	mat3 matrix;
	std::vector<Rock*> rocks;	//one group
	std::string index = "";
	std::vector<vec2> points;	// All polygon's points
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

	};
}
