#pragma once

#include <GL/glew.h>
#include <stdexcept>

class GLFrameBuffer {
public:
    GLFrameBuffer() {};
    void Creat(unsigned int width, unsigned int height);
    ~GLFrameBuffer();

    void Bind() const;
    void Unbind() const;

    GLuint GetColorAttachment() const { return colorAttachment; }

private:
    GLuint framebuffer;
    GLuint colorAttachment;
    GLuint depthStencilAttachment;
};