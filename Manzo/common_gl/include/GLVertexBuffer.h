/*
 * Rudy Castan, Seohyeon Min
 * CS200
 * Fall 2022
 */

#pragma once

#include <GL/glew.h>
#include <GLHandle.h>
#include <span>


class [[nodiscard]] GLVertexBuffer
{
    GLsizei  size          = 0;
    GLHandle buffer_handle = 0;

public:
    explicit GLVertexBuffer(GLsizei size_in_bytes);

    template <class SPAN>
    // C++20 way of saying we only want things like std::span that give us a pointer and it's size in bytes
        requires requires(SPAN s) {
            { s.data() };
            { s.size_bytes() };
    }
    // reuse 1st constructor then set our data from the span object
    GLVertexBuffer(SPAN buffer_data) : GLVertexBuffer(static_cast<GLsizei>(buffer_data.size_bytes()))
    {
        send_buffer_data_to_gpu(buffer_data.data(), size, 0);
    }

    ~GLVertexBuffer();

    GLVertexBuffer(const GLVertexBuffer&) = delete;
    GLVertexBuffer& operator=(const GLVertexBuffer&) = delete;
    GLVertexBuffer(GLVertexBuffer&& temp) noexcept;
    GLVertexBuffer& operator=(GLVertexBuffer&& temp) noexcept;

    void Use(bool bind = true) const;

    template <typename T, auto N = std::dynamic_extent>
    void SetData(std::span<T, N> buffer_data, GLsizei starting_offset = 0) const
    {
        send_buffer_data_to_gpu(buffer_data.data(), static_cast<GLsizei>(buffer_data.size_bytes()), starting_offset);
    }

    void CreateVertexBuffer(const void* data, unsigned int size);

    [[nodiscard]] GLHandle GetHandle() const noexcept
    {
        return buffer_handle;
    }

    [[nodiscard]] GLsizei GetSizeBytes() const noexcept
    {
        return size;
    }

private:
    void send_buffer_data_to_gpu(const void* data, GLsizei size_bytes, GLsizei starting_offset) const;
};
