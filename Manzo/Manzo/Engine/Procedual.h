#pragma once

#include "vec2.h"
#include <vector>

class ProceduralChain {
public:
    void Initialize(int count, float defaultSize);
    void Update(vec2 headPosition, float followSpeed);
    const std::vector<vec2>& GetPositions() const;

private:
    std::vector<vec2> positions;
    std::vector<float> circle_size;
};

