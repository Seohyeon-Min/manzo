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


void CS230::Map::ParseSVG(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening SVG file." << std::endl;
        return;
    }

    std::regex pathRegex("(?:<path[^>]*\\s+)d\\s*=\\s*\"([^\"]+)\"");
    std::smatch match;
    std::string line;
    std::string currentTag;

    while (std::getline(file, line)) {
        currentTag += line;

        if (line.find(">") != std::string::npos) {
            if (std::regex_search(currentTag, match, pathRegex)) {
                std::string pathData = match[1].str();
                std::replace(pathData.begin(), pathData.end(), ' ', ',');

                std::istringstream stream(pathData);
                std::string data;
                int count = 0;
                float last_x = 0;
                float last_y = 0;
                bool isRelative = false;

                std::vector<vec2> positions;
                while (std::getline(stream, data, ',')) {
                    if (data == "m") {
                        isRelative = true;
                        count = 0; // reset count for new relative coordinates
                        continue;
                    }
                    else if (data == "M") {
                        isRelative = false;  // absolute coordinates
                        count = 0;  // reset count for new absolute coordinates
                        continue;
                    }
                    else if (data == "z" || data == "Z") {
                        continue;  // Ignore close path commands for now
                    }

                    float x = 0, y = 0;

                    // First coordinate set (whether relative or absolute)
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
                            // Relative coordinates
                            x = last_x + std::stof(data);
                            last_x = x;
                            if (std::getline(stream, data, ',')) {
                                y = last_y + std::stof(data);
                                last_y = y;
                            }
                        }
                        else {
                            // Absolute coordinates
                            x = std::stof(data);
                            last_x = x;
                            if (std::getline(stream, data, ',')) {
                                y = std::stof(data);
                                last_y = y;
                            }
                        }
                    }

                    vec2 vec;
                    vec.x = x;
                    vec.y = -y;
                    //std::cout << "x : " << x << "       " << "y : " << y << std::endl;
                    positions.push_back(vec);
                    count++;
                }

                Polygon poly;
                poly.vertices = positions;
                poly.vertexCount = int(positions.size());
                objects.push_back(poly);
                Rock* rock = new Rock(poly);
                Engine::GetGameStateManager().GetGSComponent<CS230::GameObjectManager>()->Add(rock);
                rock->AddGOComponent(new MAP_SATCollision(poly, rock));
            }

            currentTag.clear();
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
    AddGOComponent(new CS230::Sprite("assets/images/rock.spt", this));
}

void Rock::Update(double dt)
{
    GameObject::Update(dt);
}

void Rock::Draw()
{
    GameObject::Draw();
}

void Rock::ReadRockCSV(const std::string& filename) {
    std::ifstream        file(filename);
    std::string          line, cell;

    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    if (file.is_open())
    {
        std::getline(file, line);

        while (std::getline(file, line))
        {
            std::stringstream linestream(line);

            // rock index
            std::getline(linestream, cell, ',');
            
            // hotspot_x position
            std::getline(linestream, cell, ',');

            // hotspot_y_position
            std::getline(linestream, cell, ',');

            // file path
            std::getline(linestream, cell, ',');


        }
        file.close();
    }
}