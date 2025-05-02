#pragma once

#include "vec2.h"
#include <vector>
#include "../Engine/Engine.h"

class ProceduralChain {
public:
    void Initialize(int count, float defaultSize, vec2 start_position);
    void Update(vec2 headPosition, float followSpeed);
    void Draw(const mat3& parent_matrix, DrawLayer layer);
    const std::vector<vec2>& GetPositions() const;
    void SetSizeAt(int index, float size);
    void SetAllSize(float size);

private:
    std::vector<vec2> positions;
    std::vector<float> circle_size;
};