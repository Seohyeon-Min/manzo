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
    
    if (currentMapIndex + 1 >= mapFiles.size()) return;

    currentMapIndex++;

    Map* nextMap = new Map();
    nextMap->ParseSVG(mapFiles[currentMapIndex]);
    nextMap->SetMargin(800.0f);

    //float EndY = -10000.0f;
    //nextMap->Translate({ 0, EndY });

    maps.push_back(nextMap);
    
}


void MapManager::UpdateMaps(const Math::rect& camera_boundary) {
    if (currentMapIndex < maps.size()) {
        Map* map = maps[currentMapIndex];
        map->LoadMapInBoundary(camera_boundary);

        //if(true){
        if (camera_boundary.Bottom() <= EndY + 500) {
            LoadNextMap();
        }
    }
}


//===============================================================Map

int line_number = -1;
bool level_loaded = false;
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

    int read_line_number = -1;
    while (std::getline(file, line)) {
        read_line_number++;
        if (read_line_number < line_number) {
            continue;
        }
        line_number++;
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
                break;
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
                    
                    std::cout << "Circle position || cx: " << circle_position.x << ", cy: " << circle_position.y << std::endl;
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

                Polygon original_poly = poly;
                Polygon modified_poly = poly;

                //adjusting polygon vertices
                vec2 poly_center = original_poly.FindCenter();
                std::vector<vec2> new_vertices;

                for (vec2 vertice : original_poly.vertices) {
                    vec2 new_vertice = { vertice.x - poly_center.x, vertice.y - poly_center.y };
                    new_vertices.push_back(new_vertice);
                }
                modified_poly.vertices = new_vertices;



               

                // group index
                std::string group_index = (poly.polyindex).substr(poly.polyindex.size() - 2, 2);

                // type index
                std::string type_index = (poly.polyindex).substr(0, 1);

                Rock* rock = nullptr;

                // Making Polygons into Rock
                if (type_index == "o") {

                    rock = new ObstacleRock(original_poly, modified_poly, poly_center, rotateAngle, scale);
                }
                else {
                    rock = new Rock(original_poly, modified_poly, poly_center, rotateAngle, scale);
                }

                // Making RockGroups
                if (rock_groups.empty()) {

                    RockGroup* rockgroup = new RockGroup(group_index, rotateAngle, scale);   // make new group
                    rockgroup->AddRock(rock);                                       //add poly into new group
                    
                    rock->SetRockGroup(rockgroup);
                    rock_groups.push_back(rockgroup);
                }
                else {
                    if (rock_groups.back()->GetIndex() != group_index) {                        // if poly has different index
                        RockGroup* rockgroup = new RockGroup(group_index, rotateAngle, scale);  // make new group
                        rockgroup->AddRock(rock);                                               //add poly into new group

                        rock->SetRockGroup(rockgroup);
                        rock_groups.push_back(rockgroup);
                    }
                    else {                                                              // if poly has same index
                        rock_groups.back()->AddRock(rock);
                        rock->SetRockGroup(rock_groups.back());
                    }
                }
                rocks.push_back(rock); //add rock and rockgroups in MapManager
                
            }

            //std::cout << "vertex count : " << poly.vertexCount << std::endl;
            //std::cout << "poly count : " << poly.polycount << std::endl;
            if (circle_position.x != 0.f && circle_position.y != 0.f) {
                Box* box = new Box(circle_position);
                Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->Add(box);
                std::cout << "New Circle! " << "\n";

                //for (auto& group : rock_groups) {
                //    if (group->GetIndex() == circleIndex) {//if index is equal
                //        group->AddRockPoint(rockpoint);     //add point to the group
                //        //std::cout << "Circle Added to" << group->GetIndex() << "\n";
                //    }
                //}
            }
            
            
            // Reset transforms for the next group

            
        }

        if (text is "</svg>") {
            level_loaded = true;
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

    return positions;
}





bool Map::IsOverlapping(const Math::rect& camera_boundary, const Math::rect& rock) {
    return !(camera_boundary.Right() + margin < rock.Left() ||
        camera_boundary.Left() - margin > rock.Right() ||
        camera_boundary.Top() + margin < rock.Bottom() ||
        camera_boundary.Bottom() - margin > rock.Top());
}

void Map::LoadMapInBoundary(const Math::rect& camera_boundary) {
    for (RockGroup* rockgroup : rock_groups) {
        std::vector<Rock*> rocks = rockgroup->GetRocks();

        if(!rocks.empty()){

            bool overlapping = IsOverlapping(camera_boundary, rockgroup->FindBoundary());

            if (overlapping) {
                //Add Rock in GameState
                for (auto& rock : rockgroup->GetRocks()) {

                    Polygon original_poly = rock->GetOriginalPoly();
                    Polygon modified_poly = rock->GetModifiedPoly();
                    if (!rock->IsActivated() && !rock->IsCrashed()) {
                        rock->Active(true);
                        rock->AddGOComponent(new MAP_SATCollision(modified_poly, rock));
                        Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->Add(rock);
                    }
                }

                // Add RockGroup in GameState
                if (!rockgroup->IsActivated()) {
                    rockgroup->Active(true);
                    rockgroup->SetPosition(rockgroup->FindCenterRect());
                    Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->Add(rockgroup);
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