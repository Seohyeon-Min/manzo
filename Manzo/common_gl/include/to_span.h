
// Utility.h
#pragma once

#include <span>
#include "mat3.h"
#include "color3.h"

namespace util {
    inline std::span<const float, 3 * 3> to_span(const mat3& m) {
        return std::span<const float, 3 * 3>(&m.elements[0][0], 9);
    }

    inline std::span<const float, 3> to_span(const color3& c) {
        return std::span<const float, 3>(&c.elements[0], 3);
    }
}