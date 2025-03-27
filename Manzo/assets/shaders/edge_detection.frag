#version 450 core

layout(location = 0) in vec3 vColor;
layout(location = 1) in vec2 vTextureCoordinates;

layout(location = 0) out vec4 fFragClr;

uniform sampler2D uTex2d;
uniform bool canCollideWithMouse;
uniform vec2 texelSize;  // (1.0 / texture width, 1.0 / texture height)

void main() {
    vec4 tex_color = texture(uTex2d, vTextureCoordinates);
    if (tex_color.a <= 0)
        discard;

    float alpha = tex_color.a;

    // �ֺ� �ȼ� ���� �� ��������
    float alphaLeft   = texture(uTex2d, vTextureCoordinates - vec2(texelSize.x, 0)).a;
    float alphaRight  = texture(uTex2d, vTextureCoordinates + vec2(texelSize.x, 0)).a;
    float alphaTop    = texture(uTex2d, vTextureCoordinates + vec2(0, texelSize.y)).a;
    float alphaBottom = texture(uTex2d, vTextureCoordinates - vec2(0, texelSize.y)).a;

    // ���� ����
    float edge = abs(alphaLeft - alphaRight) + abs(alphaTop - alphaBottom);
    edge = smoothstep(0.02, 0.1, edge);  // ���� ����


        vec3 glowColor = vec3(1.0, 0.8, 0.5);  // ������ Glow ����
        float glowIntensity = edge * 1.5;  // ���ϰ� ����
        tex_color.rgb = mix(tex_color.rgb, glowColor, glowIntensity);
    

    fFragClr = tex_color;
}
