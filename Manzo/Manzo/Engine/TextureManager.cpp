

#include "TextureManager.h"

#include "Engine.h"
#include "color3.h"
#include "Render.h"

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include  <stb_image.h>
#include <array>

GLTexture* CS230::TextureManager::Load(const std::filesystem::path& file_name, const GLShader* shader)
{
    auto texture_iterator = textures.find(file_name);
    if (texture_iterator != textures.end()) {
        return textures[file_name];
    }
    else {
        GLTexture* new_texture = new GLTexture(std::filesystem::path(file_name));

        new_texture->SetShader(shader);

        const float NDCwidth = ConvertToNDCWidth(new_texture->GetWidth(), Engine::window_width);
        const float NDCheight = ConvertToNDCWidth(new_texture->GetHeight(), Engine::window_height);
        GLVertexArray* model = CreatModel(NDCwidth, NDCheight);

        Engine::GetRender().LoadTextureAndModel(file_name, model);

        textures.insert(std::make_pair(file_name, new_texture));
        Engine::GetLogger().LogEvent("Loading Texture: " + file_name.string());
        return textures[file_name];
    }
}

void CS230::TextureManager::Unload()
{
    for (auto& pair : textures) {
        delete pair.second;
    }
    textures.clear();
    Engine::GetLogger().LogEvent("Clearing Textures");
}

GLVertexArray* CS230::TextureManager::CreatModel(const float width, const float height)
{
    GLVertexArray* model;
    const  float half_width = width / 2;
    const  float half_height = height / 2;
    const std::array positions = { vec2{-half_height, -half_width}, vec2{half_height, -half_width}, vec2{half_height, half_width}, vec2{-half_height, half_width} };
    constexpr std::array colors = { color3{1, 1, 1}, color3{1, 0, 0}, color3{0, 1, 0}, color3{0, 0, 1} };
    constexpr std::array<unsigned, 4> indices = { 0, 3, 1, 2 };
    constexpr std::array texture_coordinates = { vec2{0, 0}, vec2{1, 0}, vec2{1, 1}, vec2{0, 1} };

    constexpr auto positions_byte_size = static_cast<long long>(sizeof(vec2) * positions.size());
    constexpr auto colors_byte_size = static_cast<long long>(sizeof(color3) * colors.size());
    constexpr auto texture_coordinates_byte_size = static_cast<long long>(sizeof(vec2) * texture_coordinates.size());
    constexpr auto buffer_size = positions_byte_size + colors_byte_size + texture_coordinates_byte_size;

    GLVertexBuffer buffer(buffer_size);
    buffer.SetData(std::span(positions));
    buffer.SetData(std::span(colors), positions_byte_size);
    buffer.SetData(std::span(texture_coordinates), positions_byte_size + colors_byte_size);

    // Position attribute
    GLAttributeLayout position;
    position.component_type = GLAttributeLayout::Float;
    position.component_dimension = GLAttributeLayout::_2;
    position.normalized = false;
    position.vertex_layout_location = 0; // Layout location for position
    position.stride = sizeof(vec2); // Stride for position
    position.offset = 0; // Offset for position

    // Color attribute
    GLAttributeLayout color;
    color.component_type = GLAttributeLayout::Float;
    color.component_dimension = GLAttributeLayout::_3;
    color.normalized = false;
    color.vertex_layout_location = 1; // Layout location for color
    color.stride = sizeof(color3); // Stride for color
    color.offset = positions_byte_size; // Offset for color

    // Texture coordinate attribute
    GLAttributeLayout texture_coord;
    texture_coord.component_type = GLAttributeLayout::Float;
    texture_coord.component_dimension = GLAttributeLayout::_2;
    texture_coord.normalized = false;
    texture_coord.vertex_layout_location = 2; // Layout location for texture coordinates
    texture_coord.stride = sizeof(vec2); // Stride for texture coordinates
    texture_coord.offset = positions_byte_size + colors_byte_size; // Offset for texture coordinates

    model->AddVertexBuffer(std::move(buffer), { position, color, texture_coord });
    model->SetPrimitivePattern(GLPrimitive::TriangleStrip);
    GLIndexBuffer index_buffer(indices);
    model->SetIndexBuffer(std::move(index_buffer));

    return model;
}
