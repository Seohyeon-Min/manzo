#pragma once
#include "Procedual.h"
#include <unordered_map>
#include <memory>
#include <string>

class ProceduralAnimationManager {
public:
    ProceduralChain* CreateInstance(const std::string& id, int count, float defaultSize, vec2 start_position);
    ProceduralChain* Get(const std::string& id);
    void DestroyInstance(const std::string& id);
    void UpdateAll(GameObject* head, float followSpeed);
    void DrawAll(const mat3& parent_matrix, DrawLayer layer);

private:
    std::unordered_map<std::string, std::unique_ptr<ProceduralChain>> instances;
};