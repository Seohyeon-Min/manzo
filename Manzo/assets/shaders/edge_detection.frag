#version 450 core

layout(location = 0) in vec3 vColor;
layout(location = 1) in vec2 vTextureCoordinates;

layout(location = 0) out vec4 fFragClr;

uniform sampler2D uTex2d;
uniform vec2 textureSize;

void main() {
    vec2 texelSize = vec2(1.0/textureSize.x, 1.0/textureSize.y);
    vec4 tex_color = texture(uTex2d, vTextureCoordinates);
    if (tex_color.a <= 0)
        discard;

    // Glow ���� ���� Ȯ�� (��Ƽ ���ø�)
    float edge = 0.0;
    edge += abs(texture(uTex2d, vTextureCoordinates - vec2(texelSize.x * 1.0, 0)).a - tex_color.a);
    edge += abs(texture(uTex2d, vTextureCoordinates + vec2(texelSize.x * 1.0, 0)).a - tex_color.a);
    edge += abs(texture(uTex2d, vTextureCoordinates - vec2(0, texelSize.y * 1.0)).a - tex_color.a);
    edge += abs(texture(uTex2d, vTextureCoordinates + vec2(0, texelSize.y * 1.0)).a - tex_color.a);

    // �� ���� ���� ���ø�
    edge += abs(texture(uTex2d, vTextureCoordinates - vec2(texelSize.x * 2.0, 0)).a - tex_color.a);
    edge += abs(texture(uTex2d, vTextureCoordinates + vec2(texelSize.x * 2.0, 0)).a - tex_color.a);
    edge += abs(texture(uTex2d, vTextureCoordinates - vec2(0, texelSize.y * 2.0)).a - tex_color.a);
    edge += abs(texture(uTex2d, vTextureCoordinates + vec2(0, texelSize.y * 2.0)).a - tex_color.a);

    // �ε巴�� Glow ����
    edge = smoothstep(0.01, 0.1, edge);  // �β� ����

    // �� ���� Glow ȿ�� ����
    if (edge > 0.0) {
        vec3 glowColor = vec3(1.0, 0., 0.);  // ������ Glow ����
        float glowIntensity = pow(edge, 1.5) * 2.5;  // ��� ����
        tex_color.rgb += glowIntensity * glowColor;
    }

    fFragClr = tex_color;
}
