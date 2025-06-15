#version 330 core

layout(location = 0) in vec2 aPos;      // [-1,1] ������ quad ����
layout(location = 1) in vec2 aTexCoord; // �ؽ�ó ��ǥ

out vec2 vTex;

void main()
{
    gl_Position = vec4(aPos, 0.0, 1.0);
    vTex = aTexCoord;
}
