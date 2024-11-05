#include "MapManager.h"
#include "GameObjectManager.h"
#include "vec2.h"
#include <cmath>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <regex>
#include <cmath>

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
            }

            //std::cout << "vertex count : " << poly.vertexCount << std::endl;
            //std::cout << "poly count : " << poly.polycount << std::endl;
            

            // Reset transforms for the next group
            
        }
        
    }

    file.close();
    AddDrawCall();
}


void CS230::Map::AddDrawCall()
{
    for (auto& object : objects) {
        object.Draw();
    }
}

Rock::Rock(Polygon) :GameObject({ 0,0 })
{
    AddGOComponent(new CS230::Sprite("assets/images/rock1.spt", this));

}

void Rock::Update(double dt)
{
    GameObject::Update(dt);
}

void Rock::Draw()
{
    GameObject::Draw();
}