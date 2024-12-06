#pragma once

#include <GL/glew.h>
#include <stdexcept>

class GLFrameBuffer {
public:
    GLFrameBuffer(unsigned int width, unsigned int height);
    ~GLFrameBuffer();

    void Bind() const;      // 프레임 버퍼 바인딩
    void Unbind() const;    // 프레임 버퍼 언바인딩

    GLuint GetColorAttachment() const { return colorAttachment; }

private:
    GLuint framebuffer;           // OpenGL 프레임 버퍼 ID
    GLuint colorAttachment;       // 컬러 텍스처
    GLuint depthStencilAttachment; // 깊이 및 스텐실 렌더 버퍼
};