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
    void Initialize(const std::vector<int>& sizes, vec2 start_position);
    void Update(GameObject* headObject, float followSpeed);
    void Draw(const mat3& parent_matrix, DrawLayer layer);
    const std::vector<vec2>& GetPositions() const;
    void SetSizeAt(int index, float size);
    void SetAllSize(float size);
    void Clear();

private:
    std::vector<vec2> positions;
    std::vector<float> circle_size;
};