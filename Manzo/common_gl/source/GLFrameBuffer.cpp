#include "GLFrameBuffer.h"


void GLFrameBuffer::Creat(unsigned int width, unsigned int height) {
    // 프레임 버퍼 생성
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    // 컬러 텍스처 생성
    glGenTextures(1, &colorAttachment);
    glBindTexture(GL_TEXTURE_2D, colorAttachment);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorAttachment, 0);

    // 프레임 버퍼 상태 확인
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        throw std::runtime_error("Framebuffer is not complete!");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0); // 프레임 버퍼 언바인딩
}

GLFrameBuffer::~GLFrameBuffer() {
    glDeleteFramebuffers(1, &framebuffer);
    glDeleteTextures(1, &colorAttachment);
    glDeleteRenderbuffers(1, &depthStencilAttachment);
}


void GLFrameBuffer::Bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
}

void GLFrameBuffer::Unbind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
