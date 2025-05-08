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


#ifndef M_PI
#define M_PI 3.14
#endif


//MapManager

Map* MapManager::GetCurrentMap() {
    if (maps.empty()) return nullptr;
    return maps[currentMapIndex];
}

void MapManager::AddMapFile(const std::string& filename) {
    mapFiles.push_back(filename);
}


void MapManager::LoadFirstMap() {
    if (mapFiles.empty()) return;

    Map* initialMap = new Map();
    initialMap->OpenSVG(mapFiles[currentMapIndex]);
    maps.push_back(initialMap);
}

void MapManager::LoadNextMap() {
    if (currentMapIndex + 1 >= mapFiles.size()) return;

    currentMapIndex++;

    Map* nextMap = new Map();
    nextMap->SetMargin(800.0f);
    nextMap->OpenSVG(mapFiles[currentMapIndex]);    //open next svg file

    maps.push_back(nextMap);
}

void MapManager::UpdateMaps(const Math::rect& camera_boundary) {
    if (currentMapIndex < maps.size()) {
        Map* map = maps[currentMapIndex];
        map->LoadMapInBoundary(camera_boundary);

        if (camera_boundary.Bottom() <= EndY + 500) {
            //Unload Previous Map
        }
        if (currentMapIndex + 1 < maps.size()) {
            Map* nextMap = maps[currentMapIndex + 1];

            if (!nextMap->IsLevelLoaded()) {    // if next map is not loaded
                nextMap->ParseSVG();            //parse SVG file
            }
        }
    }
}

//Map
//===============================================================

Map::Map() :    pathRegex(R"(<path[^>]*\sd\s*=\s*"([^"]+))"),
                gIdRegex(R"(<g[^>]*\bid\s*=\s*"([^"]+))"),
                circleRegex(R"(circle[^>]*\bcx\s*=\s*"([^"]+))"),
                cyRegex(R"(\bcy\s*=\s*"([^"]+))"),
                cIdxRegex(R"()"),
                labelRegex(R"(inkscape:label\s*=\s*"([^"]+))"),
                transformRegex(R"(transform\s*=\s*"([^"]+))"),
                translateRegex(R"(translate\(([^,]+),\s*([^\)]+)\))"),
                rotateRegex(R"(rotate\(\s*([^\s,]+)\s*,\s*([^\s,]+)\s*,\s*([^\)]+)\s*\))"),
                matrixRegex(R"(matrix\(([^,]+),([^,]+),([^,]+),([^,]+),([^,]+),([^,]+)\))"),
                svgCloseRegex(R"(</svg>)")
{}


void Map::OpenSVG(const std::string& filename) {
    file.open(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening SVG file." << std::endl;
        return;
    }
}
int i = 0;
void Map::ParseSVG() {
    i++;
    std::cout << "Map is Parsing " << i << "st sentence."<< std::endl;
    
    if (level_loaded) {
        std::cerr << "Level is already loaded." << std::endl;
        return;
    }    

    std::string line;
    if (!std::getline(file, line)) {
        level_loaded = true; // ÆÄÀÏ ³¡
        file.close();
        std::cout << "SVG parsing completed (EOF)." << std::endl;
        return;
    }

    // </svg>
    if (std::regex_search(currentTag, svgCloseRegex)) {
        //set points
        for (auto& r_group : rock_groups) {
            r_group->SetPoints();
        }

        level_loaded = true;
        file.close();
        std::cout << "SVG parsing completed." << std::endl;
        return;
    }

    // </g>
    if (line.find("</g>") != std::string::npos) {
        IsinGroup = false;
        currentGroup = nullptr;
        polyIndex.clear();
        translate = { 0, 0 };
        rotateAngle = 0;
        rotatetranslate = { 0, 0 };
        scale = { 1.0f, 1.0f };
        IsTranslate = false;
        IsRotate = false;
        IsScale = false;
        std::cout << "Group closed." << std::endl;
        currentTag.clear();
        return;
    }

    // g id
    std::smatch match;
    if (std::regex_search(currentTag, match, gIdRegex)) {
        IsinGroup = true;
        polyIndex = match[1].str();
        std::string group_index = polyIndex.substr(polyIndex.size() - 2, 2);
        currentGroup = new RockGroup(group_index, rotateAngle, scale);
        rock_groups.push_back(currentGroup);
        std::cout << "Group created: " << group_index << std::endl;
        currentTag.clear();
        return;
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
        currentTag.clear();
        return;
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
    if (std::regex_search(currentTag, match, pathRegex)) {
        std::string pathData = match[1].str();
        std::replace(pathData.begin(), pathData.end(), ' ', ',');

        std::vector<vec2> positions2= parsePathData(pathData);   // parse path

        std::vector<vec2> positions;

        for (auto& vec : positions2) {
            if (IsinGroup) {
                // scale
                if (IsScale) {
                    vec.x *= scale.x;
                    vec.y *= scale.y;
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
                
        Polygon poly;
        poly.vertices = positions;
        poly.vertexCount = int(positions.size());
        poly.polyindex = polyIndex.empty() ? "NULL" : polyIndex;

        Rock* rock = new Rock(poly, poly, poly.FindCenter(), rotateAngle, scale);
        if (currentGroup) {
            currentGroup->AddRock(rock);
            rock->SetRockGroup(currentGroup);
        }
        rocks.push_back(rock);

        std::cout << "Path parsed & Rock created for group: " << polyIndex << std::endl;
        

        std::cout << "-----------------------------" << std::endl;
        std::cout << rotatetranslate.x << rotatetranslate.y << std::endl;
        std::cout << translate.x << translate.y << std::endl;
        std::cout << rotateAngle << std::endl;
        std::cout << "poly index : " << poly.polyindex << std::endl;
        std::cout << "-----------------------------" << std::endl;


        //===========================================Previous Codes
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

        currentTag.clear();
        return;
    }

    if (circle_position.x != 0.f && circle_position.y != 0.f) {
        Box* box = new Box(circle_position);
        Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->Add(box);
        std::cout << "New Circle! " << "\n";
    }

    // Read Next Line
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