#version 450 core

layout(location = 0) in vec3 vColor;
layout(location = 1) in vec2 vTextureCoordinates;

layout(location = 0) out vec4 fFragClr;
uniform sampler2D uTex2d;

void main() {
    vec4 tex_color = texture(uTex2d, vTextureCoordinates);
    if(tex_color.a <= 0)
        discard;
    fFragClr = vec4(0.2, 0.0, 0.5, tex_color.a);
}
//blend mode