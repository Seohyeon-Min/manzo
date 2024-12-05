#pragma once

#include <GL/glew.h>

class GLFrameBuffer {
public:
    GLFrameBuffer(unsigned int width, unsigned int height);
    ~GLFrameBuffer();

    void Bind() const;
    void Unbind() const;

    GLuint GetColorAttachment() const { return colorAttachment; }

private:
    GLuint framebuffer;
    GLuint colorAttachment;
    GLuint depthStencilAttachment;
};