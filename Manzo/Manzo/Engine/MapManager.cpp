#include "MapManager.h"
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

                std::vector<vec2> positions;

                while (std::getline(stream, data, ',')) {
                    if (data == "m" || data == "z") {
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
                        x = last_x + std::stof(data);
                        last_x = x;
                        if (std::getline(stream, data, ',')) {
                            y = last_y + std::stof(data);
                            last_y = y;
                        }
                    }
                    vec2 vec;
                    vec.x = x;
                    vec.y = y;
                    std::cout << "x : " << x << "       " << "y : " << y << std::endl;
                    positions.push_back(vec);
                    count++;
                }


                Polygon poly;
                poly.vertices = positions;
                poly.vertexCount = int(positions.size());

                objects.push_back(poly);
            }

            currentTag.clear();
        }
    }

    

    file.close();
}

std::vector<Polygon> CS230::Map::GetMapObject() {
    return objects;
}