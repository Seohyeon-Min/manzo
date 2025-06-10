#version 450 core

layout(location = 0) in vec3 vColor;
layout(location = 1) in vec2 vTextureCoordinates;

layout(location = 0) out vec4 outColor;

uniform sampler2D uTex2d;

void main()
{
    float uEmissionStrength = 1.2f;
    
    vec4 texColor = texture(uTex2d, vTextureCoordinates);

    vec3 emissive = texColor.rgb * uEmissionStrength;

    if(texColor.a <= 0)
        discard;
        
    outColor = vec4(emissive, texColor.a);
    
}