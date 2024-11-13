#pragma once
#include <array>

struct DrawSettings {
    int                   filtering_index = 0;
    std::array<int, 2>    wrap_index = { 0, 0 };
    float                 tex_coord_scale = 1.0f;
    float                 procedural_tile_size = 8.0f;
    bool                  modulate_color = false;
    bool                  apply_procedural_texture = false;
    bool                  use_texture = true;
    bool                  do_blending = false;
    bool                  animate_procedural_texture = false;
    bool                  is_UI = false;
};