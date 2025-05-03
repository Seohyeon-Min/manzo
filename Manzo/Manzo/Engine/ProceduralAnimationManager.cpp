#include "ProceduralAnimationManager.h"
#include "../Engine/Engine.h"


ProceduralChain* ProceduralAnimationManager::CreateInstance(const std::string& id, int count, float defaultSize, vec2 start_position) {
    auto chain = std::make_unique<ProceduralChain>();
    chain->Initialize(count, defaultSize, start_position);  
    instances[id] = std::move(chain);
    return instances[id].get();
}

ProceduralChain* ProceduralAnimationManager::Get(const std::string& id) {
    auto it = instances.find(id);
    if (it != instances.end())
        return it->second.get();
    return nullptr;
}

void ProceduralAnimationManager::DestroyInstance(const std::string& id) {
    instances.erase(id);
}

void ProceduralAnimationManager::UpdateAll(GameObject* head, float followSpeed) {
    for (auto& [id, chain] : instances) {
        chain->Update(head, followSpeed); // 
    }
}

void ProceduralAnimationManager::DrawAll(const mat3& parent_matrix, DrawLayer layer) {
    for (auto& [id, chain] : instances) {
        chain->Draw(parent_matrix, layer);
    }
}
