#version 450 core

layout(location = 0) in vec3 vColor; 
layout(location = 1) in vec2 vTextureCoordinates;

layout(location = 0) out vec4 fFragClr;

uniform vec3 uFillColor;
uniform float uAlpha;

void main() {
    vec4 color = vec4(uFillColor, 1.0); 
    color.a *= uAlpha;              
    fFragClr = color;             
}