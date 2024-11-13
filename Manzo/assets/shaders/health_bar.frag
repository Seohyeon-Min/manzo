#version 450 core

layout(location = 0) in vec3 vColor;
layout(location = 1) in vec2 vTextureCoordinates;

layout(location = 0) out vec4 fFragClr;
uniform sampler2D uTex2d;
uniform float uFilled;

void main() {
    vec2 texCoord = vec2(vTextureCoordinates.x, vTextureCoordinates.y);
    vec4 tex_color = texture(uTex2d, texCoord);

    if (vTextureCoordinates.x < 1.0f - uFilled) {
        discard; // Discard fragment outside the filled area
    }

    if(tex_color.a <= 0)
        discard;
    fFragClr = tex_color;
}