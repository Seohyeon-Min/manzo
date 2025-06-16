#version 330 core

layout(location = 0) in vec2 aPos;      // [-1,1] 공간의 quad 정점
layout(location = 1) in vec2 aTexCoord; // 텍스처 좌표

out vec2 vTex;

void main()
{
    gl_Position = vec4(aPos, 0.0, 1.0);
    vTex = aTexCoord;
}
