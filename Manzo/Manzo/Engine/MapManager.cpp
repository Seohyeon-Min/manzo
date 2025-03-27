/*
Copyright (C) 2023 DigiPen Institute of Technology
Reproduction or distribution of this file or its contents without
prior written consent is prohibited
File Name:  MapManager.cpp
Project:    Manzo
Author:     SeokWha Hong
Created:    September 12, 2024
*/

#include "MapManager.h"
#include "GameObjectManager.h"
#include "vec2.h"
#include "Collision.h"

#include <cmath>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <regex>
#include <cmath>
#include <algorithm>
#ifndef M_PI
#define M_PI 3.14
#endif


//MapManager
void MapManager::AddMapFile(const std::string& filename) {
    mapFiles.push_back(filename);
}

void MapManager::LoadFirstMap() {
    if (mapFiles.empty()) return;

    Map* initialMap = new Map();
    initialMap->ParseSVG(mapFiles[currentMapIndex]);
    maps.push_back(initialMap);
}

void MapManager::LoadNextMap() {
    /*
    if (currentMapIndex + 1 >= mapFiles.size()) return;

    currentMapIndex++;

    Map* nextMap = new Map();
    nextMap->ParseSVG(mapFiles[currentMapIndex]);

    float EndY = -10000.0f;
    nextMap->Translate({ 0, EndY });

    maps.push_back(nextMap);
    */
}


void MapManager::UpdateMaps(const Math::rect& camera_boundary) {
    if (currentMapIndex < maps.size()) {
        Map* map = maps[currentMapIndex];
        map->LoadMapInBoundary(camera_boundary);

        if(true){
        //if (camera_boundary.Bottom() <= EndY + 100) {
            LoadNextMap();
        }
    }
}


//===============================================================Map
std::vector<Polygon> EarClipping(const std::vector<vec2>& points);

void Map::ParseSVG(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening SVG file." << std::endl;
        return;
    }

    std::regex pathRegex(R"(<path[^>]*\sd\s*=\s*"([^"]+))");
    std::regex gIdRegex(R"(<g[^>]*\bid\s*=\s*"([^"]+))");
    std::regex circleRegex(R"(circle[^>]*\bcx\s*=\s*"([^"]+))");
    std::regex cyRegex(R"(\bcy\s*=\s*"([^"]+))");
    std::regex cIdxRegex(R"()");
    std::regex labelRegex(R"(inkscape:label\s*=\s*"([^"]+))");
    std::regex transformRegex(R"(transform\s*=\s*"([^"]+))");
    std::regex translateRegex(R"(translate\(([^,]+),\s*([^\)]+)\))");
    std::regex rotateRegex(R"(rotate\(\s*([^\s,]+)\s*,\s*([^\s,]+)\s*,\s*([^\)]+)\s*\))");
    std::regex matrixRegex(R"(matrix\(([^,]+),([^,]+),([^,]+),([^,]+),([^,]+),([^,]+)\))");

    std::smatch match;
    std::string line;
    std::string currentTag;


    vec2 translate = { 0, 0 };
    float rotateAngle = 0;
    vec2 rotatetranslate = { 0, 0 };
    vec2 scale = { 1.0f, 1.0f };

    bool IsinG = false;
    bool IsTranslate = false;
    bool IsRotate = false;
    bool IsScale = false;
    bool IsinGroup = false;
    std::string polyIndex;
    std::string circleIndex;

    while (std::getline(file, line)) {
        currentTag += line;

        if (line.find(">") != std::string::npos) {

            int pathCountInGroup = 0;
            Polygon poly;

            
           

            //g id
            if (line.find("</g>") != std::string::npos) {
                IsinGroup = false;
                polyIndex.clear();
                translate = { 0, 0 };
                rotateAngle = 0;
                rotatetranslate = { 0, 0 };
                IsTranslate = false;
                IsRotate = false;
                currentTag.clear();
                continue;
            }

            // g id
            if (std::regex_search(currentTag, match, gIdRegex)) {
                IsinGroup = true;
                polyIndex = match[1].str();
            }


            // transform
            if (std::regex_search(currentTag, match, transformRegex)) {
                std::string transformStr = match[1].str();
                std::cout << "" << std::endl;
                
                // scale
                if (std::regex_search(transformStr, match, matrixRegex)) {
                    IsScale = true;
                    float a = std::stof(match[1].str());
                    float b = std::stof(match[2].str());
                    float c = std::stof(match[3].str());
                    float d = std::stof(match[4].str());
                    float e = std::stof(match[5].str());
                    float f = std::stof(match[6].str());

                    scale.x = std::sqrt(a * a + c * c);  
                    scale.y = std::sqrt(b * b + d * d); 

                }

                // rotate
                else if (std::regex_search(transformStr, match, rotateRegex)) {
                    translate = { 0, 0 };
                    IsTranslate = false;
                    IsRotate = true;
                    rotateAngle = -std::stof(match[1].str()) * static_cast<float>(M_PI) / 180.0f;
                    rotatetranslate.x = std::stof(match[2].str());
                    rotatetranslate.y = std::stof(match[3].str());


                }
                // translate
                else if (std::regex_search(transformStr, match, translateRegex)) {
                    rotateAngle = 0;
                    rotatetranslate = { 0, 0 };
                    translate.x = std::stof(match[1].str());
                    translate.y = -std::stof(match[2].str());
                    IsTranslate = true;
                    IsRotate = false;
                }



            }
            
            //circle
            if (std::regex_search(currentTag, match, circleRegex)) {
                float x = 0;
                float y = 0;
                
                x = std::stof(match[1].str());
                if (std::regex_search(currentTag, match, cyRegex)) {
                    y = std::stof(match[1].str());
                    
                    //std::cout << "Circle position || cx: " << circle_position.x << ", cy: " << circle_position.y << std::endl;
                }
                else {
                    //std::cerr << "Error: cy not found for circle with cx: " << circle_position.x << std::endl;
                }
                if (std::regex_search(currentTag, match, labelRegex)) {
                    circleIndex = match[1].str();
                    //std::cout << "Circle index : " << circleIndex << std::endl;

                }
                vec2 vec = { x, -y };
                circle_position = vec;
            }

            //poly position
            std::string pathData;
            while (std::regex_search(currentTag, match, pathRegex)) {   // path parsing
                pathData = match[1].str();
                std::replace(pathData.begin(), pathData.end(), ' ', ',');   //replace blank to ','

                std::vector<vec2> positions2= parsePathData(pathData);   // parse path

                std::vector<vec2> positions;

                for (auto& vec : positions2) {
                    if (IsinGroup) {
                        // scale
                        if (IsScale) {
                            vec.x += scale.x;
                            vec.y += scale.y;
                            //std::cout << "Scaled!"<<std::endl;
                        }
                        //rotate
                        if (IsRotate) {
                            vec.x += rotatetranslate.x;
                            vec.y += rotatetranslate.y;

                            float rotatedX = vec.x * std::cos(rotateAngle) - vec.y * std::sin(rotateAngle);
                            float rotatedY = vec.x * std::sin(rotateAngle) + vec.y * std::cos(rotateAngle);
                            vec.x = rotatedX;
                            vec.y = rotatedY;


                        }
                        // translate
                        if (IsTranslate) {
                            vec.x += translate.x;
                            vec.y += translate.y;
                        }
                        positions.push_back(vec);
                    }
                }
                

                poly.vertices = positions;
                poly.vertexCount = int(positions.size());
                poly.polycount = pathCountInGroup > 0 ? pathCountInGroup : 0;
                if (polyIndex.empty() == true) {
                    polyIndex = "NULL";
                }
                else {
                    poly.polyindex = polyIndex;
                }

                pathCountInGroup++;
                currentTag.clear();
                std::cout << "-----------------------------" << std::endl;
                std::cout << rotatetranslate.x << rotatetranslate.y << std::endl;
                std::cout << translate.x << translate.y << std::endl;
                std::cout << rotateAngle << std::endl;
                std::cout << "poly index : " << poly.polyindex << std::endl;
                std::cout << "-----------------------------" << std::endl;

                // Making Polygons into Rock
                Rock* rock = new Rock(poly);
                rocks.push_back(rock);
                RockGroup* rockgroup = new RockGroup(poly.polyindex);
                rockgroup->AddRock(rock);
                rock->SetRockGroup(rockgroup);
                rock_groups.push_back(rockgroup);
                

            }

            //std::cout << "vertex count : " << poly.vertexCount << std::endl;
            //std::cout << "poly count : " << poly.polycount << std::endl;
            
            if (circle_position.x != 0.f && circle_position.y != 0.f && circleIndex != "") {
                RockPoint* rockpoint = new RockPoint(circle_position, circleIndex);
                Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->Add(rockpoint);
                //std::cout << "New Circle! " << "\n";

                for (auto& group : rock_groups) {
                    if (group->GetIndex() == circleIndex) {//if index is equal
                        group->AddRockPoint(rockpoint);     //add point to the group
                        //std::cout << "Circle Added to" << group->GetIndex() << "\n";
                    }
                }
            }
           
            // Reset transforms for the next group

            
        }

    }

    //debugging & matching index, points
    for (auto& r_group : rock_groups) {

        std::cout << "Group Index : " << r_group->GetIndex() << "\n";
        std::cout << "Group Rocks Size : " << r_group->GetRocks().size() << "\n";
        /*std::cout << "Group Position: " << r_group->GetPosition().x << "," << r_group->GetPosition().y << "\n";
        std::cout << "Group Index : " << r_group->GetIndex() << "\n";
        std::cout << "Group Rocks Size : " << r_group->GetRocks().size() << "\n";
        std::cout << "Group Moving Rocks Size : " << r_group->GetMovingRocks().size() << "\n";
        std::cout << "How Many Points? : " << r_group->GetPoints().size() << "\n";
        */

        //r_group->MatchIndex();
        r_group->SetPoints();
        //std::cout <<"How Many Points? : " << r_group->GetPoints().size() <<"\n";
    }
    file.close();
}

// Ear Clipping
float PolygonArea(const std::vector<vec2>& points) {
    float area = 0;
    for (size_t i = 0; i < points.size(); ++i) {
        const vec2& p1 = points[i];
        const vec2& p2 = points[(i + 1) % points.size()];
        area += (p1.x * p2.y) - (p2.x * p1.y);
    }
    return area * 0.5f;
}

constexpr bool IsConvex(const vec2& a, const vec2& b, const vec2& c) noexcept {
    vec2 ab = b - a;
    vec2 bc = c - b;
    return cross(ab, bc) > 0;
}

bool PointInTriangle(const vec2& p, const vec2& a, const vec2& b, const vec2& c) {
    vec2 ab = b - a, bc = c - b, ca = a - c;
    vec2 ap = p - a, bp = p - b, cp = p - c;

    float cross1 = cross(ab, ap);
    float cross2 = cross(bc, bp);
    float cross3 = cross(ca, cp);

    
    return (cross1 < 0 && cross2 < 0 && cross3 < 0) || (cross1 > 0 && cross2 > 0 && cross3 > 0);
}

bool isPolygonConvex(const std::vector<vec2>& points) {
    size_t n = points.size();
    if (n < 3) return false;
    bool convex = true;
    // 모든 연속된 세 점에 대해 convex 여부 확인
    for (size_t i = 0; i < n; i++) {
        size_t prev = (i == 0) ? n - 1 : i - 1;
        size_t next = (i + 1) % n;
        if (!IsConvex(points[prev], points[i], points[next])) {
            convex = false;
            break;
        }
    }
    return convex;
}

float TriangleMinAngle(const vec2& a, const vec2& b, const vec2& c) {
    auto computeAngle = [&](const vec2& v1, const vec2& v2) -> float {
        float dot = v1.x * v2.x + v1.y * v2.y;
        float mag1 = std::sqrt(v1.x * v1.x + v1.y * v1.y);
        float mag2 = std::sqrt(v2.x * v2.x + v2.y * v2.y);
        float cosTheta = dot / (mag1 * mag2);
        cosTheta = std::max(-1.0f, std::min(1.0f, cosTheta));
        return std::acos(cosTheta) * 180.0f / 3.14159265f;
        };
    float angleA = computeAngle(b - a, c - a);
    float angleB = computeAngle(a - b, c - b);
    float angleC = computeAngle(a - c, b - c);
    return std::min({ angleA, angleB, angleC });
}


std::vector<Polygon> EarClipping(const std::vector<vec2>& points) {
    std::vector<Polygon> triangles;
    size_t n = points.size();
    if (n < 3) return triangles;

    // convex면 팬 삼각분할
    if (isPolygonConvex(points)) {
        for (size_t i = 1; i < n - 1; ++i) {
            Polygon tri;
            tri.vertices = { points[0], points[i], points[i + 1] };
            triangles.push_back(tri);
        }
        return triangles;
    }

    // concave면 Ear Clipping 수행
    std::vector<vec2> remaining_points = points;
    while (remaining_points.size() > 3) {
        bool ear_found = false;
        for (size_t i = 0; i < remaining_points.size(); ++i) {
            size_t prev = (i == 0) ? remaining_points.size() - 1 : i - 1;
            size_t next = (i + 1) % remaining_points.size();
            vec2 a = remaining_points[prev];
            vec2 b = remaining_points[i];
            vec2 c = remaining_points[next];
            if (!IsConvex(a, b, c))
                continue;
            bool has_internal = false;
            for (size_t j = 0; j < remaining_points.size(); ++j) {
                if (j == prev || j == i || j == next)
                    continue;
                if (PointInTriangle(remaining_points[j], a, b, c)) {
                    has_internal = true;
                    break;
                }
            }
            if (has_internal)
                continue;
            // Ear 발견 → 삼각형 추가하고 b 제거
            Polygon tri;
            tri.vertices = { a, b, c };
            triangles.push_back(tri);
            remaining_points.erase(remaining_points.begin() + i);
            ear_found = true;
            break;
        }
        if (!ear_found) {
            std::cout << "EarClipping failed: Invalid polygon or ear not found." << std::endl;
            Polygon poly;
            poly.vertices = points;
            triangles.push_back(poly);
            return triangles;
        }
    }
    if (remaining_points.size() == 3) {
        Polygon tri;
        tri.vertices = { remaining_points[0], remaining_points[1], remaining_points[2] };
        triangles.push_back(tri);
        std::cout << "Added final triangle." << std::endl;
    }
    return triangles;
}


// Map Parsing
std::vector<vec2> Map::parsePathData(const std::string& pathData) {

    std::istringstream stream(pathData);
    std::string data;

    
    float last_x = 0, last_y = 0; // former position
    bool isRelative = false;

    std::vector<vec2> positions;

    while (std::getline(stream, data, ',')) {
        if (std::isalpha(data[0])) {  // check is command
            currentCommand = data[0];
            isRelative = std::islower(currentCommand);  // is Relative?
            continue;
        }

        float x = 0.0f, y = 0.0f;

        if (currentCommand == 'm' || currentCommand == 'M') {  
            x = std::stof(data);  
            if (std::getline(stream, data, ',')) { 
                
                y = std::stof(data);
                last_x = isRelative ? last_x + x : x;
                last_y = isRelative ? last_y + y : y;
                positions.push_back({ last_x, -last_y });
                currentCommand = isRelative ? 'l' : 'L';  // Treat as L or l
            }
        }
        else if (currentCommand == 'l' || currentCommand == 'L') {  
            x = std::stof(data);
            if (std::getline(stream, data, ',')) {
                
                y = std::stof(data);
                last_x = isRelative ? last_x + x : x;
                last_y = isRelative ? last_y + y : y;
                positions.push_back({ last_x, -last_y });
            }
        }
        else if (currentCommand == 'v' || currentCommand == 'V') { 
            
            y = std::stof(data);
            last_y = isRelative ? last_y + y : y;
            positions.push_back({ last_x, -last_y });  
        }
        else if (currentCommand == 'h' || currentCommand == 'H') {  
            
            x = std::stof(data);
            last_x = isRelative ? last_x + x : x;
            positions.push_back({ last_x, -last_y });  
        }
        else if (currentCommand == 'z' || currentCommand == 'Z') {  
            if (!positions.empty()) {
                positions.push_back(positions.front());  //close
            }
        }
    }
    /*
    for (const auto& pos : positions) {
        std::cout << "Position: (" << pos.x << ", " << pos.y << ")" << std::endl;
    }*/

    return positions;
}


void Map::MakeMovingRockGroups(MovingRock* moving_rock, Polygon poly) {

    // Making RockGroups
    if (rock_groups.empty()) {
        RockGroup* rockgroup = new RockGroup(poly.polyindex);   // make new group
        rockgroup->AddMovingRock(moving_rock);                                       //add poly into new group

        moving_rock->SetRockGroup(rockgroup);
        Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->Add(rockgroup);
        rock_groups.push_back(rockgroup);
    }
    else {
        if (rock_groups.back()->GetIndex() != poly.polyindex) {             // if poly has different index
            RockGroup* rockgroup = new RockGroup(poly.polyindex);           // make new group
            rockgroup->AddMovingRock(moving_rock);                                       //add poly into new group

            moving_rock->SetRockGroup(rockgroup);
            //Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->Add(rockgroup);
            rock_groups.push_back(rockgroup);
        }
        else {                                                              // if poly has same index
            rock_groups.back()->AddMovingRock(moving_rock);
            moving_rock->SetRockGroup(rock_groups.back());
        }
    }
}


bool Map::IsOverlapping(const Math::rect& camera_boundary, const Math::rect& rock) {
    return !(camera_boundary.Right() + margin < rock.Left() ||
        camera_boundary.Left() - margin > rock.Right() ||
        camera_boundary.Top() + margin < rock.Bottom() ||
        camera_boundary.Bottom() - margin > rock.Top());
}

void Map::LoadMapInBoundary(const Math::rect& camera_boundary) {
    for (auto& rockgroup : rock_groups) {
        std::vector<Rock*> rocks = rockgroup->GetRocks();

        if(!rocks.empty()){
            Polygon poly2 = rocks[0]->GetPolygon();

            bool overlapping = IsOverlapping(camera_boundary, poly2.FindBoundary());

            if (overlapping) {
                for (Rock* rock : rocks) {
                    //Add Rock in GameState
                    if (!rock->IsActivated()) {

                        rock->Active(true);
                        rock->AddGOComponent(new MAP_SATCollision(poly2, rock));
                        Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->Add(rock);
                        //std::cout << "Rock Added to GameState!!!!!!!!!!!!!!!!!!"<<"\n";
                    }
                }

                // Add RockGroup in GameState
                if (!rockgroup->IsActivated()) {

                    rockgroup->Active(true);
                    Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->Add(rockgroup);
                    //std::cout << "RockGroup Added to GameState!!!!!!!!!!!!!!!!!!" << "\n";
                }
                
            }
            else {
                for (Rock* rock : rocks) {
                    //Remove Rock in GameState
                    if (rock->IsActivated()) {

                        rock->Active(false);
                        Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->Remove(rock);
                        //std::cout << "Unloaded Rock!!!!!!!!!!!!!!!!!" << "\n";
                    }
                }

                // Remove RockGroup in GameState
                if (rockgroup->IsActivated()) {

                    rockgroup->Active(false);
                    Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->Remove(rockgroup);
                }
            }
        }
        
    }
}

void Map::Translate(const vec2& offset) {
    for (Rock* rock : rocks) {
        vec2 pos = rock->GetPosition();
        rock->SetPosition({ pos.x, pos.y + offset.y });
    }

    for (RockGroup* rockgroup : rock_groups) {
        vec2 pos = rockgroup->GetPosition();
        rockgroup->SetPosition({ pos.x, pos.y + offset.y });
    }
}


void Map::UnloadAll() {
    for (Rock* rock : rocks) {
        if (!rock->IsActivated()) {
            delete rock;
        }
    }
    for (RockGroup* rockgroup : rock_groups) {
        if (!rockgroup->IsActivated()) {
            delete rockgroup;
        }
    }
}