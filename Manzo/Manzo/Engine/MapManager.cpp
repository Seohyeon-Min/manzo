/*
Copyright (C) 2023 DigiPen Institute of Technology
Reproduction or distribution of this file or its contents without
prior written consent is prohibited
File Name:  MapManager.cpp
Project:    Manzo
Author:     SeokWha Hong
Created:    September 12, 2024
*/
#define _CRT_SECURE_NO_WARNINGS
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "stb_image.h"
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
#include "Camera.h"


//MapManager
void MapManager::AddMapFile(const std::string& filename) {
    mapFiles.push_back(filename);
}

void MapManager::LoadFirstMap() {
    if (mapFiles.empty()) return;

    Map* initialMap = new Map();
    initialMap->ParseSVG(mapFiles[currentMapIndex]);
    initialMap->LoadPNG();
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
                //std::cout << "" << std::endl;
                
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
                //std::cout << "-----------------------------" << std::endl;
                //std::cout << rotatetranslate.x << rotatetranslate.y << std::endl;
                //std::cout << translate.x << translate.y << std::endl;
                //std::cout << rotateAngle << std::endl;
                //std::cout << "poly index : " << poly.polyindex << std::endl;
                //std::cout << "-----------------------------" << std::endl;

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


                // Making Polygons into Rock
                Rock* rock = new Rock(original_poly, modified_poly, poly_center, rotateAngle, scale);

               


                std::string group_index = (poly.polyindex).substr(poly.polyindex.size() - 2, 2);
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
                rocks.push_back(rock);
                //add rock and rockgroups in MapManager
                

            }

            //std::cout << "vertex count : " << poly.vertexCount << std::endl;
            //std::cout << "poly count : " << poly.polycount << std::endl;
            if (circle_position.x != 0.f && circle_position.y != 0.f) {
                Box* box = new Box(circle_position);
                Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->Add(box);
                //std::cout << "New Circle! " << "\n";

                //for (auto& group : rock_groups) {
                //    if (group->GetIndex() == circleIndex) {//if index is equal
                //        group->AddRockPoint(rockpoint);     //add point to the group
                //        //std::cout << "Circle Added to" << group->GetIndex() << "\n";
                //    }
                //}
            }
            
            
            // Reset transforms for the next group

            
        }

    }

    //debugging & matching index, points
    for (auto& r_group : rock_groups) {

        //std::cout << "Group Index : " << r_group->GetIndex() << "\n";
        //std::cout << "Group Rocks Size : " << r_group->GetRocks().size() << "\n";
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

void Map::FillPolygonScanline(const std::vector<ivec2>& polygon, std::vector<unsigned char>& data, int width, int height) {
    if (polygon.size() < 3) return;

    // 폴리곤의 Y 범위 구하기
    int minY = polygon[0].y;
    int maxY = polygon[0].y;
    for (const auto& vertex : polygon) {
        minY = std::min(minY, vertex.y);
        maxY = std::max(maxY, vertex.y);
    }

    // 각 스캔라인에 대해
    for (int y = minY; y <= maxY; ++y) {
        std::vector<int> intersections;

        // 모든 에지와의 교점 찾기
        for (size_t i = 0; i < polygon.size(); ++i) {
            int j = int((i + 1) % polygon.size());
            ivec2 p1 = polygon[i];
            ivec2 p2 = polygon[j];

            // 수평선 제외
            if (p1.y == p2.y) continue;

            // y가 에지 범위에 있는지 확인
            if (y >= std::min(p1.y, p2.y) && y < std::max(p1.y, p2.y)) {
                // 교점의 x 좌표 계산
                int x = p1.x + (y - p1.y) * (p2.x - p1.x) / (p2.y - p1.y);
                intersections.push_back(x);
            }
        }

        // 교점들을 x 좌표로 정렬
        std::sort(intersections.begin(), intersections.end());

        // 교점 쌍 사이를 채우기 (even-odd rule)
        for (size_t i = 0; i < intersections.size(); i += 2) {
            if (i + 1 < intersections.size()) {
                int startX = intersections[i];
                int endX = intersections[i + 1];

                for (int x = startX; x <= endX; ++x) {
                    if (x >= 0 && x < width && y >= 0 && y < height) {
                        data[y * width + x] = 255;
                    }
                }
            }
        }
    }
}

void Map::DrawRockLine(ivec2 p0, ivec2 p1, std::vector<unsigned char>& data, int width, int height) {
    int dx = abs(p1.x - p0.x);
    int dy = abs(p1.y - p0.y);
    int sx = p0.x < p1.x ? 1 : -1;
    int sy = p0.y < p1.y ? 1 : -1;
    int err = dx - dy;

    int x = p0.x;
    int y = p0.y;

    while (true) {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            data[y * width + x] = 255;
        }
        if (x == p1.x && y == p1.y) break;
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x += sx;
        }
        if (e2 < dx) {
            err += dx;
            y += sy;
        }
    }
}


void Map::LoadMapInBoundary(const Math::rect& camera_boundary) {
    // 1. 화면 해상도(또는 원하는 마스크 해상도) 지정
    int width = Engine::window_width;   // 또는 원하는 해상도
    int height = Engine::window_height;
    data.resize(width * height, 0);

    // 2. 월드→마스크 변환 람다 (카메라 뷰 기준)
    auto wv = [&](vec2 worldPos) -> ivec2 {
        float world_left = camera_boundary.Left();
        float world_right = camera_boundary.Right();
        float world_bottom = camera_boundary.Bottom();
        float world_top = camera_boundary.Top();

        float world_width = world_right - world_left;
        float world_height = world_top - world_bottom;

        int maskX = static_cast<int>(((worldPos.x - world_left) / world_width) * width);
        int maskY = static_cast<int>(((world_top - worldPos.y) / world_height) * height);

        return {
            std::clamp(maskX, 0, width - 1),
            std::clamp(maskY, 0, height - 1)
        };
        };

    // 3. RockGroup 순회 및 장애물 맵 마스킹
    for (RockGroup* rockgroup : rock_groups) {
        std::vector<Rock*> rocks = rockgroup->GetRocks();
        if (!rocks.empty()) {
            bool overlapping = IsOverlapping(camera_boundary, rockgroup->FindBoundary());
            if (overlapping) {
                for (auto& rock : rocks) {
                    if (!rock->IsActivated()) {
                        rock->Active(true);
                        rock->AddGOComponent(new MAP_SATCollision(rock->GetModifiedPoly(), rock));
                        Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->Add(rock);
                    }
                }
                if (!rockgroup->IsActivated()) {
                    rockgroup->Active(true);
                    rockgroup->SetPosition(rockgroup->FindCenterRect());
                    Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->Add(rockgroup);
                }
                // --- 장애물 맵에 Rock 폴리곤 마스킹 ---
                for (Rock* rock : rocks) {
                    Polygon poly = rock->GetOriginalPoly();
                    std::vector<ivec2> maskPoly;
                    for (auto& vert : poly.vertices) {
                        maskPoly.push_back(wv(vert));
                    }
                    for (size_t i = 0; i < maskPoly.size(); ++i) {
                        int j = int((i + 1) % maskPoly.size());
                        DrawRockLine(maskPoly[i], maskPoly[j], data, width, height);
                    }
                }
            }
            else {
                for (Rock* rock : rocks) {
                    if (rock->IsActivated()) {
                        rock->Active(false);
                        Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->Remove(rock);
                    }
                }
                if (rockgroup->IsActivated()) {
                    rockgroup->Active(false);
                    Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->Remove(rockgroup);
                }
            }
        }
    }

    // 4. OpenGL 텍스처로 업로드(최초 1회 생성, 이후 데이터만 갱신)
    if (obstacleTex == 0) {
        glGenTextures(1, &obstacleTex);
        glBindTexture(GL_TEXTURE_2D, obstacleTex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data.data());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }
    else {
        glBindTexture(GL_TEXTURE_2D, obstacleTex);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RED, GL_UNSIGNED_BYTE, data.data());
    }
    glBindTexture(GL_TEXTURE_2D, 0);

    // 5. 디버그용 PNG로 저장
    stbi_write_png("obstacle.png", width, height, 1, data.data(), width);
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

void Map::LoadPNG()
{
    unsigned char* imgData = stbi_load("assets/maps/mask.png", &width, &height, &channels, 0);

    if (!imgData) {
        std::cerr << "Failed to load map mask .png file.\n";
        return;
    }

    mask.resize(height, std::vector<bool>(width));

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int idx = (y * width + x) * channels;
            unsigned char r = imgData[idx];
            unsigned char g = imgData[idx + 1];
            unsigned char b = imgData[idx + 2];
            int flippedY = height - 1 - y;
            mask[flippedY][x] = (r < 10 && g < 10 && b < 10);

            if (mask[flippedY][x]) {
                valid_spawn_positions.emplace_back(x, flippedY);
            }
        }
    }

    std::cout << "Load PNG successfully" << std::endl;
    //for (int i = 0; i < height; i++)
    //{
    //    for (int j = 0; j < width; j++)
    //    {
    //        if(mask[i][j] ==  1) std::cout << "mask[" << i << "][" << j << "] : " << mask[i][j] << std::endl;
    //    } 
    //}
    stbi_image_free(imgData);

}

vec2 Map::MaskToWorld(int maskX, int maskY)
{
    float world_left = 0.0f;
    float world_top = 0.0f;
    float world_right = 4970.0f;
    float world_bottom = -6000.0f;

    float world_width = world_right - world_left;
    float world_height = world_bottom - world_top;

    float x = (float)maskX / (float)width * world_width + world_left;
    float y = (float)( maskY) / (float)height * world_height + world_top;

    return { x, y };
}

ivec2 Map::WorldToMask(vec2 worldPos)
{
    float world_left = 0.0f;
    float world_top = 0.0f;
    float world_right = 4970.0f;
    float world_bottom = -6000.0f;

    float world_width = world_right - world_left;
    float world_height = world_bottom - world_top;


    int maskX = static_cast<int>(std::floor(((worldPos.x - world_left) / world_width) * width));
    int maskY = static_cast<int>(std::floor(((worldPos.y - world_top) / world_height) * height));

    maskX = std::clamp(maskX, 0, width - 1);
    maskY = std::clamp(maskY, 0, height - 1);

    //int maskX = static_cast<int>(((worldPos.x - world_left) / world_width) * width);
    //int maskY = static_cast<int>(((worldPos.y - world_top) / world_height) * height);

    //maskX = std::max(0, std::min(maskX, width - 1));
    //maskY = std::max(0, std::min(maskY, height - 1));

    return { maskX, maskY };
}

bool Map::IsMaskTrue(vec2 worldPos)
{
    ivec2 maskPos = WorldToMask(worldPos);
    return mask[maskPos.y][maskPos.x];
}

vec2 Map::Spawn() {
    std::uniform_int_distribution<> distX(0, width - 1);
    std::uniform_int_distribution<> distY(0, height - 1);
    int max_attempts = 1000;

    for (int i = 0; i < max_attempts; ++i) {
        int maskX = distX(gen);
        int maskY = distY(gen);

        vec2 worldPos = MaskToWorld(maskX, maskY);

        if (IsMaskTrue(worldPos)) {
            return worldPos;
        }
    }

    return { 0.0f, 0.0f };
}
