/*
Copyright (C) 2024 DigiPen Institute of Technology
Reproduction or distribution of this file or its contents without
prior written consent is prohibited
File Name:  Rect.cpp
Project:    CS230 Engine
Author:     Jonathan Holmes, Gyuwon Na
Created:    March 8, 2023
Updated:    May 11, 2024
*/

#include "Rect.h"
#include <utility>

int Math::rect::Left() const noexcept {
    return std::min(point_1.x, point_2.x);
}

int Math::rect::Right() const noexcept {
    return std::max(point_1.x, point_2.x);
}

int Math::rect::Top() const noexcept {
    return std::max(point_1.y, point_2.y);
}

int Math::rect::Bottom() const noexcept {
    return std::min(point_1.y, point_2.y);
}

int Math::irect::Left() const noexcept {
    return std::min(point_1.x, point_2.x);
}

int Math::irect::Right() const noexcept {
    return std::max(point_1.x, point_2.x);
}

int Math::irect::Top() const noexcept {
    return std::max(point_1.y, point_2.y);
}

int Math::irect::Bottom() const noexcept {
    return std::min(point_1.y, point_2.y);
}
