#version 330 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexCoord;

out vec2 vScreenPos;

uniform vec2 uScreenSize;

void main() {
    gl_Position = vec4(aPos, 0.0, 1.0);
    
    // NDC�� ��ũ�� ��ǥ�� ��ȯ
    vScreenPos = (aPos + 1.0) * 0.5 * uScreenSize;
}
