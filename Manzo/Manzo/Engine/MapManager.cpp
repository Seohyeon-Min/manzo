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


void CS230::Map::ParseSVG(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening SVG file." << std::endl;
        return;
    }

    std::regex pathRegex(R"(<path[^>]*\sd\s*=\s*"([^"]+))");
    std::regex gIdRegex(R"(<g[^>]*\bid\s*=\s*"([^"]+))");
    std::regex transformRegex(R"(transform\s*=\s*"([^"]+))");
    std::regex translateRegex(R"(translate\(([^,]+),\s*([^\)]+)\))");
    std::regex rotateRegex(R"(rotate\(\s*([^\s,]+)\s*,\s*([^\s,]+)\s*,\s*([^\)]+)\s*\))");
    std::smatch match;
    std::string line;
    std::string currentTag;


    vec2 translate = { 0, 0 };
    float rotateAngle = 0;
    vec2 rotatetranslate = { 0, 0 };

    bool IsinG = false;
    bool Istranslate = false;
    bool IsRotate = false;
    bool IsinGroup = false; 
    std::string polyIndex;

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
                Istranslate = false;
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
               

                // rotate
                if (std::regex_search(transformStr, match, rotateRegex)) {
                    translate = { 0, 0 };
                    Istranslate = false;
                    IsRotate = true;
                    rotateAngle = -std::stof(match[1].str()) * static_cast<float>(M_PI) / 180.0f;
                    rotatetranslate.x = std::stof(match[2].str());
                    rotatetranslate.y = std::stof(match[3].str());

                   
                }
                // translat
                else if (std::regex_search(transformStr, match, translateRegex)) {
                    rotateAngle = 0;
                    rotatetranslate = { 0, 0 };
                    translate.x = std::stof(match[1].str());
                    translate.y = -std::stof(match[2].str());
                    Istranslate = true;
                    IsRotate = false;
                }
                

               
            }
            
            

            // 
            std::string pathData;
            while (std::regex_search(currentTag, match, pathRegex)) {
                pathData = match[1].str();
                std::replace(pathData.begin(), pathData.end(), ' ', ',');

                std::istringstream stream(pathData);
                std::string data;
                float last_x = 0, last_y = 0;
                int count = 0;
                bool isRelative = false;

                std::vector<vec2> positions;
                while (std::getline(stream, data, ',')) {
                    if (data == "m") {
                        isRelative = true;
                        count = 0;
                        continue;
                    }
                    else if (data == "M") {
                        isRelative = false;
                        count = 0;
                        continue;
                    }
                    else if (data == "z" || data == "Z") {
                        continue;
                    }

                    float x = 0, y = 0;
                    if (count < 1) {
                        x = std::stof(data);
                        last_x = x;
                        if (std::getline(stream, data, ',')) {
                            y = std::stof(data);
                            last_y = y;
                        }
                    }
                    else {
                        if (isRelative) {
                            x = last_x + std::stof(data);
                            last_x = x;
                            if (std::getline(stream, data, ',')) {
                                y = last_y + (std::stof(data));
                                last_y = y;
                            }
                        }
                        else {
                            x = std::stof(data);
                            last_x = x;
                            if (std::getline(stream, data, ',')) {
                                y = (std::stof(data));
                                last_y = y;
                            }
                        }
                    }

                    std::cout << x << "  |  " << y << std::endl;
                    vec2 vec = { x, -y };
                    if(IsinGroup){
                        //rotate
                        if(IsRotate){
                            vec.x += rotatetranslate.x;
                            vec.y += rotatetranslate.y;

                            float rotatedX = vec.x * std::cos(rotateAngle) - vec.y * std::sin(rotateAngle);
                            float rotatedY = vec.x * std::sin(rotateAngle) + vec.y * std::cos(rotateAngle);
                            vec.x = rotatedX;
                            vec.y = rotatedY;

                        
                        }
                        // translate
                        if (Istranslate) {
                            vec.x += translate.x;
                            vec.y += translate.y;
                        }
                    }

                    positions.push_back(vec);
                    count++;
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
                Rock* rock = new Rock(poly);
                Engine::GetGameStateManager().GetGSComponent<CS230::GameObjectManager>()->Add(rock);
                rock->AddGOComponent(new MAP_SATCollision(poly, rock));
                
                pathCountInGroup++;  
                currentTag.clear();
                std::cout << "-----------------------------" << std::endl;
                std::cout << rotatetranslate.x << rotatetranslate.y << std::endl;
                std::cout << translate.x << translate.y << std::endl;
                std::cout << rotateAngle << std::endl;
                std::cout << "poly index : " << poly.polyindex << std::endl;
                std::cout << "-----------------------------" << std::endl;


                if (rock_groups.empty()) {
                    RockGroup* rockgroup = new RockGroup(poly.polyindex);   // make new group
                    rockgroup->AddRock(rock);                                       //add poly into new group
                    rockgroup->SetRotation(rotateAngle);
                    //rockgroup->SetScale();

                    rock->SetRockGroup(rockgroup);
                    Engine::GetGameStateManager().GetGSComponent<CS230::GameObjectManager>()->Add(rockgroup);
                    rock_groups.push_back(rockgroup);
                }
                else {
                    if (rock_groups.back()->GetIndex() != poly.polyindex) {             // if poly has different index
                        RockGroup* rockgroup = new RockGroup(poly.polyindex);   // make new group
                        rockgroup->AddRock(rock);                                       //add poly into new group
                        rockgroup->SetRotation(rotateAngle);
                        //rockgroup->SetScale();

                        rock->SetRockGroup(rockgroup);
                        
                        Engine::GetGameStateManager().GetGSComponent<CS230::GameObjectManager>()->Add(rockgroup);
                        rock_groups.push_back(rockgroup);
                    }
                    else {                                                              // if poly has same index
                        rock_groups.back()->AddRock(rock);
                    }
                }

                
            }
            
            //std::cout << "vertex count : " << poly.vertexCount << std::endl;
            //std::cout << "poly count : " << poly.polycount << std::endl;
            

            // Reset transforms for the next group
            
        }
        
    }

    
    for (auto& r_group : rock_groups) {
        r_group->MatchIndex();
        r_group->SetPoints();
        
        /*std::cout <<"Group Position: " << r_group->GetPosition().x << "," << r_group->GetPosition().y<<"\n";
        std::cout <<"Group Index : " << r_group->GetIndex()<<"\n";
        std::cout <<"Group Rocks Size : " << r_group->GetRocks().size() <<"\n";*/
        //std::cout <<"How Many Points? : " << r_group->GetPoints().size() <<"\n";
    }
    
    file.close();
    //AddDrawCall();
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

Rock::Rock(Polygon poly) :GameObject({ 0,0 }), poly(poly)
{
    //this->SetCenter();  //set rock position
}

void Rock::Update(double dt)
{
    GameObject::Update(dt);
}

void Rock::Draw()
{
    GameObject::Draw();
}

void Rock::SetCenter() {
    vec2 center = { 0, 0 };
    std::vector<vec2> vertices = this->GetPolygon().vertices;
    for (vec2 vertice : vertices) {
        center.x += vertice.x;
        center.y += vertice.y;
    }
    center.x /= vertices.size();
    center.y /= vertices.size();
    SetPosition(center);
}

RockGroup::RockGroup(const std::string& index) :GameObject({ 0,0 }), index(index)
{}

void RockGroup::Update(double dt)
{
    CS230::GameObject::Update(dt);
}

void RockGroup::Draw()
{
    CS230::GameObject::Draw();
}

bool RockGroup::MatchIndex()
{
    std::ifstream file("assets/images/rock/rock.csv");
    std::string line, cell;
    if (!file.is_open()) {
        std::cerr << "Failed to Open CSV." << std::endl;
        return false;
    }

    if (file.is_open()) {
        while (std::getline(file, line)) {
            std::stringstream linestream(line);
            std::string index, x_str, y_str, file_path;

            std::getline(linestream, index, ',');
            std::string polyind = (this->index).substr(0, 4);

            if (index == polyind) {
                std::getline(linestream, file_path, ',');
                SetPosition(FindCenter());
                AddGOComponent(new CS230::Sprite(file_path, this));

                return true;

            }
        }

    }
    std::cerr << "Index not found in the file." << std::endl;
    return false;
}

vec2 RockGroup::FindCenter() {  // Calculate texture's position.
    vec2 center = { 0, 0 };
    vec2 minPoint = rocks[0]->GetPolygon().vertices[1];
    vec2 maxPoint = rocks[0]->GetPolygon().vertices[0];

    for (auto& rock : rocks) {
        Polygon poly = rock->GetPolygon();
        minPoint.x = std::min(minPoint.x, poly.FindBoundary().Left());
        minPoint.y = std::min(minPoint.y, poly.FindBoundary().Bottom());
        maxPoint.x = std::max(maxPoint.x, poly.FindBoundary().Right());
        maxPoint.y = std::max(maxPoint.y, poly.FindBoundary().Top());
    }
    center.x = (minPoint.x + maxPoint.x) /2;
    center.y = (minPoint.y + maxPoint.y) /2;
    return center;
}

void RockGroup::SetPoints() {
    for (auto& rock : rocks) {
        Polygon poly = rock->GetPolygon();
        for (int i = 0; i < poly.vertexCount; i++) {
            points.push_back(poly.vertices[i]);
        }
   }
}

bool RockGroup::CanCollideWith(GameObjectTypes other_object)
{
    switch (other_object) {
    case GameObjectTypes::Ship:
        return true;
        break;
    }

    return false;
}

void RockGroup::ResolveCollision(GameObject* other_object)
{
    if (other_object->Type() == GameObjectTypes::Ship) {
        auto* collision_edge = this->GetGOComponent<CS230::RectCollision>();
        if (collision_edge == nullptr) {
            // maybe an error?
        }
        for (auto& rock : this->GetRocks()) {
            //rock->SetVelocity({-100, 0});
            //other_object->SetVelocity({0, 0});
        }
        
    }
}
