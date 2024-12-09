#pragma once

#include <GL/glew.h>
#include <stdexcept>

class GLFrameBuffer {
public:
    GLFrameBuffer(unsigned int width, unsigned int height);
    ~GLFrameBuffer();

    void Bind() const;      // ������ ���� ���ε�
    void Unbind() const;    // ������ ���� ����ε�

    GLuint GetColorAttachment() const { return colorAttachment; }

private:
    GLuint framebuffer;           // OpenGL ������ ���� ID
    GLuint colorAttachment;       // �÷� �ؽ�ó
    GLuint depthStencilAttachment; // ���� �� ���ٽ� ���� ����
};