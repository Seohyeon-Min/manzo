#version 450 core

layout(location = 0) in vec3 vColor;
layout(location = 1) in vec2 vTextureCoordinates;

layout(location = 0) out vec4 fFragClr;

uniform vec3 uFillColor;
uniform float uAlpha;
uniform sampler2D uTex2d;

void main() {
    vec4 texColor = texture(uTex2d, vTextureCoordinates);
    texColor.a *= uAlpha;

    if (texColor.a < 0.1) {
        discard;
    }

    fFragClr = texColor;
}
