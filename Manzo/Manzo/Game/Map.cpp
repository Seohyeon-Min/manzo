
#define _CRT_SECURE_NO_WARNINGS
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

/*
Copyright (C) 2023 DigiPen Institute of Technology
Reproduction or distribution of this file or its contents without
prior written consent is prohibited
File Name:  Map.cpp
Project:    Manzo
Author:     Won Kim
Created:    June 12, 2025
*/

#include "Map.h"
#include <string.h>
#include "vec2.h"
#include "stb_image.h"
#include "..\Engine\MapManager.h"
#include "..\Engine\GameObjectManager.h"

#include "..\Engine\Collision.h"

#ifndef M_PI
#define M_PI 3.14
#endif

Map::Map(const std::string& filename, Math::rect map_boundary) :
        pathRegex(R"(<path[^>]*\sd\s*=\s*"([^"]+))"),
        gIdRegex(R"(<g[^>]*\bid\s*=\s*"([^"]+))"),
        circleRegex(R"(circle[^>]*\bcx\s*=\s*"([^"]+))"),
        cyRegex(R"(\bcy\s*=\s*"([^"]+))"),
        cIdxRegex(R"()"),
        labelRegex(R"(inkscape:label\s*=\s*"([^"]+))"),
        transformRegex(R"xxx(transform\s*=\s*"([^"]+)")xxx"),
        translateRegex(R"(translate\(([^,]+),\s*([^\)]+)\))"),
        rotateRegex(R"(rotate\(\s*([^\s,]+)\s*,\s*([^\s,]+)\s*,\s*([^\)]+)\s*\))"),
        matrixRegex(R"(matrix\(([^,]+),([^,]+),([^,]+),([^,]+),([^,]+),([^,]+)\))"),
        pathIdRegex(R"xxx(id="([^"]+)")xxx"),
        fillColorRegex(R"(fill:\s*(#[0-9a-fA-F]+);)"),
        file_path(filename),
        map_boundary(map_boundary)
{

    size_t start = file_path.find_last_of('/') + 1;
    size_t end = file_path.find_last_of('.');
    this->map_index = file_path.substr(start, end - start);

    std::random_device rd;
    gen = std::mt19937(rd());
}


void Map::OpenSVG() {
    map_file.open(this->file_path);
    if (!map_file.is_open()) {
        Engine::GetLogger().LogError(file_path + "Error opening SVG file.");
        return;
    }
}

void Map::ParseSVG() {

    if (level_loaded) {
        Engine::GetLogger().LogError("Level is already loaded.");
        return;
    }

    std::string line;
    std::string currentTag;

    do {
        if (!std::getline(map_file, line)) {
            level_loaded = true;
            map_file.close();
            return;
        }
        currentTag += line;
    } while (currentTag.find("/>") == std::string::npos && currentTag.find('>') == std::string::npos);


    // </svg>
    if (currentTag.find("</svg>") != std::string::npos) {
        for (auto& r_group : rock_groups) {
            r_group->SetPoints();
        }

        level_loaded = true;
        map_file.close();
        Engine::GetLogger().LogDebug(file_path + "file's parsing completed.");
        return;
    }

    std::smatch match;

    // </g>
    if (currentTag.find("</g>") != std::string::npos) {
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
        return;
    }

    // g id
    //std::cout << "Current Tag : " << currentTag << "\n";
    if (std::regex_search(currentTag, match, gIdRegex)) {
        IsinGroup = true;
        std::string group_index = match[1].str();
        currentGroup = new RockGroup(group_index, map_index, rotateAngle, scale);
        rock_groups.push_back(currentGroup);
        //std::cout << "Group created: " << group_index << std::endl;

    }

    // transform
    if (std::regex_search(currentTag, match, transformRegex)) {
        std::string transformStr = match[1].str();

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
        else if (std::regex_search(transformStr, match, rotateRegex)) {
            translate = { 0, 0 };
            IsTranslate = false;
            IsRotate = true;
            rotateAngle = -std::stof(match[1].str()) * static_cast<float>(M_PI) / 180.0f;
            rotatetranslate.x = std::stof(match[2].str());
            rotatetranslate.y = std::stof(match[3].str());
        }
        else if (std::regex_search(transformStr, match, translateRegex)) {
            rotateAngle = 0;
            rotatetranslate = { 0, 0 };
            translate.x = std::stof(match[1].str());
            translate.y = std::stof(match[2].str());
            IsTranslate = true;
            IsRotate = false;
        }

        return;
    }

    // circle
    if (std::regex_search(currentTag, match, circleRegex)) {
        float x = std::stof(match[1].str());
        float y = 0;
        if (std::regex_search(currentTag, match, cyRegex)) {
            y = std::stof(match[1].str());
        }

        if (std::regex_search(currentTag, match, labelRegex)) {
            circleIndex = match[1].str();
        }

        circle_position = { x, -y };
        return;
    }


    //path ID
    if (std::regex_search(currentTag, match, pathIdRegex)) {
        polyIndex = match[1].str();

    }

    // path
    if (std::regex_search(currentTag, match, pathRegex)) {
        pathData = match[1].str();
        std::replace(pathData.begin(), pathData.end(), ' ', ',');

        std::vector<vec2> positions_temp = parsePathData(pathData);     //point position before applying group information
        std::vector<vec2> positions;                                // point position after applying group information

        for (auto& vec : positions_temp) {
            if (IsinGroup) {
                if (IsScale) {
                    vec.x *= scale.x;
                    vec.y *= scale.y;
                }
                if (IsRotate) {
                    vec.x += rotatetranslate.x;
                    vec.y += rotatetranslate.y;

                    float rotatedX = vec.x * std::cos(rotateAngle) - vec.y * std::sin(rotateAngle);
                    float rotatedY = vec.x * std::sin(rotateAngle) + vec.y * std::cos(rotateAngle);
                    vec.x = rotatedX;
                    vec.y = rotatedY;
                }
                if (IsTranslate) {
                    vec.x += translate.x;
                    vec.y += translate.y;
                }
                vec.y = -vec.y;
                positions.push_back(vec);
            }

        }

        Polygon poly;
        poly.vertices = positions;
        poly.vertexCount = int(positions.size());
        poly.polyindex = polyIndex.empty() ? "NULL" : polyIndex;


        /*std::cout << "-----------------------------" << std::endl;
        std::cout << "Rotate Translate : " << rotatetranslate.x << ", " << rotatetranslate.y << std::endl;
        std::cout << "Translate : " << translate.x << ", " << translate.y << std::endl;
        std::cout << "Rotate : " << rotateAngle << std::endl;
        std::cout << "poly index : " << poly.polyindex << std::endl;
        std::cout << "-----------------------------" << std::endl;*/


        Polygon original_poly = poly;   //for collision
        Polygon modified_poly = poly;   //for object position

        if (std::regex_search(currentTag, match, fillColorRegex)) {
            fillColor = match[1].str();
        }

        vec2 poly_center = original_poly.FindCenter();

        std::vector<vec2> new_vertices;
        for (vec2 vertice : original_poly.vertices) {
            new_vertices.push_back({ vertice.x - poly_center.x, vertice.y - poly_center.y });
        }
        modified_poly.vertices = new_vertices;

        
        if (fillColor == obstacleColor) {
            ObstacleRock* rock = new ObstacleRock(original_poly, modified_poly, original_poly.FindCenter(), rotateAngle, scale);  //generate obstacle rock
            
            if (currentGroup) {
                currentGroup->AddRock(dynamic_cast<ObstacleRock*>(rock));
                dynamic_cast<ObstacleRock*>(rock)->SetRockGroup(currentGroup);
                
            }
            obstacle_rocks.push_back(dynamic_cast<ObstacleRock*>(rock));
        }
        else {                                                  
            Rock* rock = new Rock(original_poly, modified_poly, original_poly.FindCenter(), rotateAngle, scale);  //generate general rock
            if (currentGroup) {
                currentGroup->AddRock(dynamic_cast<Rock*>(rock));
                dynamic_cast<Rock*>(rock)->SetRockGroup(currentGroup);
            }
            rocks.push_back(dynamic_cast<Rock*>(rock));
        }
        
        return;
    }

    if (circle_position.x != 0.f && circle_position.y != 0.f) {
        Box* box = new Box(circle_position);
        Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->Add(box);
        //std::cout << "New Circle! " << "\n";
    }
}

// Map Parsing
std::vector<vec2> Map::parsePathData(const std::string& pathData) {
    std::istringstream stream(pathData);
    std::string _data;

    float last_x = 0, last_y = 0; // former position
    bool isRelative = false;

    std::vector<vec2> positions;

    while (std::getline(stream, _data, ',')) {
        if (std::isalpha(_data[0])) {  // check is command
            currentCommand = _data[0];
            isRelative = std::islower(currentCommand);  // is Relative?
            continue;
        }

        float x = 0.0f, y = 0.0f;

        if (currentCommand == 'm' || currentCommand == 'M') {
            x = std::stof(_data);
            if (std::getline(stream, _data, ',')) {

                y = std::stof(_data);
                last_x = isRelative ? last_x + x : x;
                last_y = isRelative ? last_y + y : y;
                positions.push_back({ last_x, last_y });
                currentCommand = isRelative ? 'l' : 'L';  // Treat as L or l
            }
        }
        else if (currentCommand == 'l' || currentCommand == 'L') {
            x = std::stof(_data);
            if (std::getline(stream, _data, ',')) {

                y = std::stof(_data);
                last_x = isRelative ? last_x + x : x;
                last_y = isRelative ? last_y + y : y;
                positions.push_back({ last_x, last_y });
            }
        }
        else if (currentCommand == 'v' || currentCommand == 'V') {

            y = std::stof(_data);
            last_y = isRelative ? last_y + y : y;
            positions.push_back({ last_x, last_y });
        }
        else if (currentCommand == 'h' || currentCommand == 'H') {

            x = std::stof(_data);
            last_x = isRelative ? last_x + x : x;
            positions.push_back({ last_x, last_y });
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
    return !(camera_boundary.Right() < rock.Left() ||
        camera_boundary.Left() > rock.Right() ||
        camera_boundary.Top() < rock.Bottom() ||
        camera_boundary.Bottom() > rock.Top());
}

bool Map::IsOverlappingMargin(const Math::rect& camera_boundary, const Math::rect& rock) {
    return !(camera_boundary.Right() + margin < rock.Left() ||
        camera_boundary.Left() - margin > rock.Right() ||
        camera_boundary.Top() + margin < rock.Bottom() ||
        camera_boundary.Bottom() - margin > rock.Top());
}

void Map::LoadMapInBoundary(const Math::rect& camera_boundary) {

    int width = Engine::window_width;
    int height = Engine::window_height;
    data.resize(width * height, 0);

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


    for (RockGroup* rockgroup : rock_groups) {
        std::vector<Rock*> rocks = rockgroup->GetRocks();

        if (!rocks.empty()) {

            bool overlapping = IsOverlappingMargin(camera_boundary, rockgroup->FindBoundary());

            if (overlapping) {
                //Add Rock in GameState
                for (auto& rock : rockgroup->GetRocks()) {

                    Polygon original_poly = rock->GetOriginalPoly();
                    std::vector<ivec2> maskPoly;
                    for (auto& vert : original_poly.vertices) {
                        maskPoly.push_back(wv(vert));
                    }
                    FillPolygonScanline(maskPoly, data, width, height);

                    Polygon modified_poly = rock->GetModifiedPoly();
                    if (!rock->IsActivated() && !rock->GetRockGroup()->IsCrashed()) {
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

void Map::UnloadAll() {
    for (Rock* rock : rocks) {
        if (rock->IsActivated()) {
            rock->Active(false);
            Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->Remove(rock);
            rock->Destroy();
            rock = nullptr;
            delete rock;

        }
        else 
        rock->Destroy();
        rock = nullptr;
        delete rock;
    }
    for (RockGroup* rockgroup : rock_groups) {
        if (rockgroup->IsActivated()) {
            rockgroup->Active(false);
            Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->Remove(rockgroup);
            rockgroup->Destroy();
            rockgroup = nullptr;
            delete rockgroup;

        }
        else
            rockgroup->Destroy();
            rockgroup = nullptr;
            delete rockgroup;
    }
}

void Map::UnloadCrashedRock() {
    for (RockGroup* rockgroup : rock_groups) {
        if (rockgroup->IsCrashed()) {
            for (Rock* rock : rockgroup->GetRocks()) {

                rock->Active(false);
                Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->Remove(rock);
                rock->Destroy();
                rock = nullptr;
                delete rock;
            }
            rockgroup->Active(false);
            Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->Remove(rockgroup);
            rockgroup->Destroy();
            rockgroup = nullptr;
            delete rockgroup;
        }
            
    }
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

void Map::LoadPNG()
{
    std::string filename_str = "assets/images/rock/" + map_index + "/mask.png";
    const char* filename = filename_str.c_str();
    unsigned char* imgData = stbi_load(filename, &width, &height, &channels, 0);

    if (!imgData) {

        Engine::GetLogger().LogError("Failed to load " + filename_str + " file");
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

    Engine::GetLogger().LogDebug("Load PNG successfully");
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
    float y = (float)(maskY) / (float)height * world_height + world_top;

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
