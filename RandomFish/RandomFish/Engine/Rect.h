/*
Copyright (C) 2023 DigiPen Institute of Technology
Reproduction or distribution of this file or its contents without
prior written consent is prohibited
File Name:  Rect.h
Project:    CS230 Engine
Author:     Jonathan Holmes
Created:    March 8, 2023
Updated:    May 11, 2024
*/

#pragma once
#include "Vec2.h"
#include "cmath"

namespace Math {
    struct [[nodiscard]] rect {
        Math::vec2 point_1{ 0.0, 0.0 };
        Math::vec2 point_2{ 0.0, 0.0 };

        vec2 Size() const noexcept {
            return {
                static_cast<double>(Right() - Left()),
                static_cast<double>(std::abs(Top() - Bottom()))
            };
        }

        int Left() const noexcept;
        int Right() const noexcept;
        int Top() const noexcept;
        int Bottom() const noexcept;
    };


    struct [[nodiscard]] irect {
        Math::ivec2 point_1{ 0, 0 };
        Math::ivec2 point_2{ 0, 0 };

        ivec2 Size() const noexcept {
            return {
                Right() - Left(),
                static_cast<int>(std::abs(Top() - Bottom()))
            };
        }

        int Left() const noexcept;
        int Right() const noexcept;
        int Top() const noexcept;
        int Bottom() const noexcept;
    };
}

