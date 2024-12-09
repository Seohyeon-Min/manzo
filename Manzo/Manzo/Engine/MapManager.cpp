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


std::vector<Polygon> EarClipping(const std::vector<vec2>& points);

void CS230::Map::ParseSVG(const std::string& filename) {
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
                            std::cout << "Scaled!"<<std::endl;
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

                objects.push_back(poly);

                pathCountInGroup++;
                currentTag.clear();
                std::cout << "-----------------------------" << std::endl;
                std::cout << rotatetranslate.x << rotatetranslate.y << std::endl;
                std::cout << translate.x << translate.y << std::endl;
                std::cout << rotateAngle << std::endl;
                std::cout << "poly index : " << poly.polyindex << std::endl;
                std::cout << "-----------------------------" << std::endl;

                // Rock Point

                //std::vector<Polygon> Polys = EarClipping(positions);
                //for (Polygon& poly : Polys) {
                    Rock* rock = new Rock(poly);
                    Engine::GetGameStateManager().GetGSComponent<CS230::GameObjectManager>()->Add(rock);
                    rock->AddGOComponent(new MAP_SATCollision(poly, rock));

                    // Making RockGroups
                    if (rock_groups.empty()) {
                        RockGroup* rockgroup = new RockGroup(poly.polyindex);   // make new group
                        rockgroup->AddRock(rock);                                       //add poly into new group

                        rock->SetRockGroup(rockgroup);
                        Engine::GetGameStateManager().GetGSComponent<CS230::GameObjectManager>()->Add(rockgroup);
                        rock_groups.push_back(rockgroup);
                    }
                    else {
                        if (rock_groups.back()->GetIndex() != poly.polyindex) {             // if poly has different index
                            RockGroup* rockgroup = new RockGroup(poly.polyindex);           // make new group
                            rockgroup->AddRock(rock);                                       //add poly into new group

                            rock->SetRockGroup(rockgroup);
                            Engine::GetGameStateManager().GetGSComponent<CS230::GameObjectManager>()->Add(rockgroup);
                            rock_groups.push_back(rockgroup);
                        }
                        else {                                                              // if poly has same index
                            rock_groups.back()->AddRock(rock);
                            rock->SetRockGroup(rock_groups.back());
                        }
                    }

                


                

                
            }

            //std::cout << "vertex count : " << poly.vertexCount << std::endl;
            //std::cout << "poly count : " << poly.polycount << std::endl;
            
            if (circle_position.x != 0.f && circle_position.y != 0.f && circleIndex != "") {
                RockPoint* rockpoint = new RockPoint(circle_position, circleIndex);
                Engine::GetGameStateManager().GetGSComponent<CS230::GameObjectManager>()->Add(rockpoint);
                std::cout << "New Circle! " << "\n";

                for (auto& group : rock_groups) {
                    if (group->GetIndex() == circleIndex) {//if index is equal
                        group->AddRockPoint(rockpoint);     //add point to the group
                        std::cout << "Circle Added to" << group->GetIndex() << "\n";
                    }
                }
            }
           
            // Reset transforms for the next group

             //Add RockPoints to the Rock Group
            

        }

    }


    for (auto& r_group : rock_groups) {
        r_group->MatchIndex();
        r_group->SetPoints();
        std::cout <<"Group Position: " << r_group->GetPosition().x << "," << r_group->GetPosition().y<<"\n";
        std::cout <<"Group Index : " << r_group->GetIndex()<<"\n";
        std::cout <<"Group Rocks Size : " << r_group->GetRocks().size() <<"\n";
        //std::cout <<"How Many Points? : " << r_group->GetPoints().size() <<"\n";
    }
    file.close();
}


void CS230::Map::AddDrawCall()
{
    for (auto& object : objects) {
        object.Draw();
    }
    for (auto& rock_group : rock_groups) {
        rock_group->Draw();
    }
}

// Ear Clipping
constexpr bool IsConvex(const vec2& a, const vec2& b, const vec2& c) noexcept {
    vec2 ab = b - a;
    vec2 bc = c - b;
    return cross(ab, bc) < 0;
}

bool PointInTriangle(const vec2& p, const vec2& a, const vec2& b, const vec2& c) {
    vec2 ab = b - a, bc = c - b, ca = a - c;
    vec2 ap = p - a, bp = p - b, cp = p - c;

    float cross1 = cross(ab, ap);
    float cross2 = cross(bc, bp);
    float cross3 = cross(ca, cp);

   
    return (cross1 < 0 && cross2 < 0 && cross3 < 0) || (cross1 > 0 && cross2 > 0 && cross3 > 0);
}
std::vector<Polygon> EarClipping(const std::vector<vec2>& points) {

    // for convex polygon
    std::vector<Polygon> triangles;

    for (size_t i = 1; i < points.size() - 1; ++i) {
        Polygon triangle;
        triangle.vertices = { points[0], points[i], points[i + 1] };
        triangles.push_back(triangle);
    }

    return triangles;

    // for the concave polygon
    /*
    std::vector<vec2> remaining_points = points;
    std::vector<Polygon> triangles;


    while (remaining_points.size() > 3) {
        bool ear_found = false;

        for (size_t i = 0; i < remaining_points.size(); ++i) {
            size_t prev = (i == 0) ? remaining_points.size() - 1 : i - 1;
            size_t next = (i + 1) % remaining_points.size();

            vec2 a = remaining_points[prev];
            vec2 b = remaining_points[i];
            vec2 c = remaining_points[next];


            if (!IsConvex(a, b, c)) continue;


            bool has_internal_point = false;
            for (size_t j = 0; j < remaining_points.size(); ++j) {
                if (j == prev || j == i || j == next) continue;
                if (PointInTriangle(remaining_points[j], a, b, c)) {
                    has_internal_point = true;
                    break;
                }
            }

            if (has_internal_point) continue;

            //add triangle
            Polygon triangle;
            triangle.vertices = { a, b, c };
            triangles.push_back(triangle);

            //eleminate current point
            remaining_points.erase(remaining_points.begin() + i);
            ear_found = true;
            break;
        }

        if (!ear_found) {
            throw std::runtime_error("EarClipping failed: Invalid polygon or input");
        }
    }

    // add last triangle
    Polygon triangle;
    triangle.vertices = { remaining_points[0], remaining_points[1], remaining_points[2] };
    triangles.push_back(triangle);

    return triangles;*/

}


// Map Parsing
std::vector<vec2> CS230::Map::parsePathData(const std::string& pathData) {

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



