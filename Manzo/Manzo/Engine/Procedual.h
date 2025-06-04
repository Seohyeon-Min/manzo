#pragma once

#include "vec2.h"
#include <vector>
#include "../Engine/Engine.h"
#include "../Engine/Lerp.h"
#include "../Engine/Engine.h"
#include "../Engine/GameObject.h"

class ProceduralChain {
public:
    
    void Initialize(int count, float defaultSize, vec2 start_position);
    void Initialize(const std::vector<int>& sizes, vec2 start_position, const std::vector<std::string>& texturePaths);
    void Update(GameObject* headObject, float followSpeed);
    void Draw(const mat3& parent_matrix, DrawLayer layer);
    const vec2 GetPositions(int index, const mat3& parent_matrix) const;
    void SetSizeAt(int index, float size);
    void SetAllSize(float size);
    void Clear();

private:
    std::vector<vec2> positions;
    std::vector<float> circle_size;
    std::vector<GLTexture*> textures;
};