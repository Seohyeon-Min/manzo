#version 450 core

layout(location = 0) in vec3 vColor;
layout(location = 1) in vec2 vTextureCoordinates;

layout(location = 0) out vec4 fFragClr;

uniform sampler2D uTex2d;
uniform float uAlpha;     // 알파값 (0.0 ~ 1.0 범위)

void main() {
    vec4 tex_color = texture(uTex2d, vTextureCoordinates);

    tex_color.a *= uAlpha;



    fFragClr = tex_color;
}
