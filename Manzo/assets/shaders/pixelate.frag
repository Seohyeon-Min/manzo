#version 450 core

layout(location = 0) in vec3 vColor;
layout(location = 1) in vec2 vTextureCoordinates;

layout(location = 0) out vec4 fFragClr;
uniform sampler2D uTex2d;
uniform float uPixelSize;

void main() {
    vec2 texCoord = vec2(vTextureCoordinates.x, vTextureCoordinates.y);
    vec2 pixelatedCoord = floor(texCoord / uPixelSize) * uPixelSize;

    vec4 tex_color = texture(uTex2d, pixelatedCoord);
    
    if(tex_color.a <= 0)
        discard;

    fFragClr = tex_color;
}