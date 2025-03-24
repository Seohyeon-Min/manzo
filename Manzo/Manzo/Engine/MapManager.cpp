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

                //rocks.push_back(poly);

                pathCountInGroup++;
                currentTag.clear();
                std::cout << "-----------------------------" << std::endl;
                std::cout << rotatetranslate.x << rotatetranslate.y << std::endl;
                std::cout << translate.x << translate.y << std::endl;
                std::cout << rotateAngle << std::endl;
                std::cout << "poly index : " << poly.polyindex << std::endl;
                std::cout << "-----------------------------" << std::endl;

                // Rock Point
                //=======ear clipping=========

                //RockGroup* rockgroup = new RockGroup(poly.polyindex);   // make new rockgroup

                std::vector<Polygon> Polys = Triangulation(positions);

                int i = 0;
                for (auto& pol : Polys) {
                    Rock* rock = new Rock(pol);
                    //rock->AddGOComponent(new MAP_SATCollision(pol, rock));
                    rocks.push_back(rock);
                    std::string index = poly.polyindex + std::to_string(i);
                    RockGroup* rockgroup = new RockGroup(index);

                    rockgroup->AddRock(rock);                                       //add poly into new group

                    rock->SetRockGroup(rockgroup);

                    rock_groups.push_back(rockgroup);
                    i += 1;
                }

                //rock_groups.push_back(rockgroup);

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
    }
    file.close();
}


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
                        std::cout << "Rock Added to GameState!!!!!!!!!!!!!!!!!!"<<"\n";
                    }
                }

                // Add RockGroup in GameState
                if (!rockgroup->IsActivated()) {

                    rockgroup->Active(true);
                    Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->Add(rockgroup);
                    std::cout << "RockGroup Added to GameState!!!!!!!!!!!!!!!!!!" << "\n";
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

std::vector<Polygon> Map::Triangulation(std::vector<vec2> positions) {
    std::vector<Polygon> triangles;

    // Make Super Triangle
    vec2 p1 = { -10000, -10000 }, p2 = { 10000, -10000 }, p3 = { 0, 10000 };
    Polygon big_triangle;
    big_triangle.vertices = { p1, p2, p3 };
    triangles.push_back(big_triangle);

    for (const vec2& position : positions) {
        std::vector<Polygon> badTriangles;
        //std::set<std::pair<Point, Point>> edgeSet;

        for (auto& t : triangles) {
            if (t.isPointInsideCircumcircle(p)) {
                badTriangles.push_back(t);
                edgeSet.insert({ t.a, t.b });
                edgeSet.insert({ t.b, t.c });
                edgeSet.insert({ t.c, t.a });
            }
        }

        // remove bad triangles
        triangles.erase(std::remove_if(triangles.begin(), triangles.end(), [&](Polygon& t) {
            return std::find(badTriangles.begin(), badTriangles.end(), t) != badTriangles.end();
            }), triangles.end());

        // fill the hole with new triangles
        for (auto& edge : edgeSet) {
            triangles.push_back({ edge.first, edge.second, p });
        }
    }

    triangles.erase(std::remove_if(triangles.begin(), triangles.end(), [&](Triangle& t) {
        return (t.a == p1 || t.a == p2 || t.a == p3 ||
            t.b == p1 || t.b == p2 || t.b == p3 ||
            t.c == p1 || t.c == p2 || t.c == p3);
        }), triangles.end());

    return triangles;

}