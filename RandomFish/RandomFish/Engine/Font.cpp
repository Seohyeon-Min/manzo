/*
Copyright (C) 2023 DigiPen Institute of Technology
Reproduction or distribution of this file or its contents without
prior written consent is prohibited
File Name:  Font.cpp
Project:    CS230 Engine
Author:     Jonathan Holmes, Gyuwon Na
Created:    March 8, 2023
Updated:    May 11, 2024
*/

#include "Font.h"
#include "Engine.h"


unsigned int CS230::Font::GetPixel(Math::ivec2 texel)
{
    if (texel.x >= 0 && texel.x < original_image.width && texel.y >= 0 && texel.y < original_image.height) {
        int index = (texel.y * original_image.width + texel.x) * 4;

        unsigned char* pixel_data = static_cast<unsigned char*>(original_image.data);

        return (pixel_data[index + 3] << 24) | (pixel_data[index] << 16) | (pixel_data[index + 1] << 8) | pixel_data[index + 2];
    }
    else {
        return 0x00000000;
    }
}

CS230::Font::Font(const std::filesystem::path& file_name) : texture(file_name) {
    const unsigned int white = 0xFFFFFFFF;

    original_image = LoadImage(file_name.string().c_str());

    //The top left pixel MUST be white (0xFFFFFFFF)
    unsigned int color = GetPixel({ 0, 0 });
    if (color == white) {
        FindCharRects();
    }
    else {
        Engine::GetLogger().LogError("Font " + file_name.string() + " could not be loaded");
        return;
    }
    UnloadImage(original_image);
}

void CS230::Font::FindCharRects() {
    unsigned int check_color = GetPixel({ 0, 0 });
    unsigned int next_color;

    int height = texture.GetSize().y;

    int x = 1;
    for (int index = 0; index < num_chars; index++) {
        int width = 0;

        do {
            width++;
            next_color = GetPixel({ x + width, 0 });
        } while (check_color == next_color);

        check_color = next_color;

        char_rects[index].point_2 = { x + width - 1, 1 };
        char_rects[index].point_1 = { x, char_rects[index].point_2.y + height - 1 };
        x += width;
    }
}

Math::irect& CS230::Font::GetCharRect(char c) {
    if (c >= ' ' && c <= 'z') {
        return char_rects[c - ' '];
    }
    else {
        Engine::GetLogger().LogError("Char '" + std::to_string(c) + "' not found");
        return char_rects[0];
    }
}

Math::ivec2 CS230::Font::MeasureText(std::string text) {
    Math::ivec2 text_size = { 0, 0 };

    for (char c : text) {
        if (c == ' ') {
            text_size.x += char_rects[0].Size().x;
        }
        else {
            const Math::irect& char_rect = GetCharRect(c);
            text_size.x += char_rect.Size().x;
            text_size.y = std::max(text_size.y, char_rect.Size().y);
        }
    }

    return text_size;
}

void CS230::Font::DrawChar(Math::TransformationMatrix& matrix, char c, unsigned int color) {
    const Math::irect& display_rect = GetCharRect(c);
    const Math::ivec2 top_left_texel = { display_rect.point_1.x, display_rect.point_2.y };
    if (c != ' ') {
        const auto to_center = Math::TranslationMatrix(Math::vec2(-display_rect.Size().x / 2.0, -display_rect.Size().y / 2.0));
        const auto flip = Math::ScaleMatrix(Math::vec2{ 1, -1 });
        const auto to_bottomleft = Math::TranslationMatrix(Math::vec2(display_rect.Size().x / 2.0, display_rect.Size().y / 2.0));
        const auto flip_quad = to_bottomleft * flip * to_center;
        texture.Draw(matrix * flip_quad, top_left_texel, display_rect.Size(), color);
    }
    matrix *= Math::TranslationMatrix(Math::ivec2{ display_rect.Size().x, 0 });
}

CS230::Texture* CS230::Font::PrintToTexture(const std::string& text, unsigned int color) {
    const Math::ivec2 text_size = MeasureText(text);
    auto& texture_manager = Engine::GetTextureManager();
    texture_manager.StartRenderTextureMode(text_size.x, text_size.y);
    Math::TransformationMatrix matrix;
    for (const char c : text) {
        DrawChar(matrix, c, color);
    }
    return texture_manager.EndRenderTextureMode();
}


