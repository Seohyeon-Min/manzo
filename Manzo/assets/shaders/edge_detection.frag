#version 450 core

layout(location = 0) in vec3 vColor;
layout(location = 1) in vec2 vTextureCoordinates;

layout(location = 0) out vec4 fFragClr;

uniform sampler2D uTex2d;
uniform vec2 textureSize;  
uniform bool canCollide;
uniform bool isColliding;

void main() {
    vec2 texelSize = vec2(1.0 / textureSize.x, 1.0 / textureSize.y);
    vec4 tex_color = texture(uTex2d, vTextureCoordinates);
    if (tex_color.a <= 0)
        discard;

    if (isColliding && canCollide) {
        float alpha = tex_color.a;

        // �ֺ� �ȼ� ���� �� ��������
        float alphaLeft   = texture(uTex2d, vTextureCoordinates - vec2(texelSize.x, 0)).a;
        float alphaRight  = texture(uTex2d, vTextureCoordinates + vec2(texelSize.x, 0)).a;
        float alphaTop    = texture(uTex2d, vTextureCoordinates + vec2(0, texelSize.y)).a;
        float alphaBottom = texture(uTex2d, vTextureCoordinates - vec2(0, texelSize.y)).a;

        // ���� ����
        float edge = abs(alphaLeft - alphaRight) + abs(alphaTop - alphaBottom);
        edge = smoothstep(0.02, 0.9, edge);  // ���� ����

        // ����þ� �� ����
        // ����þ� Ŀ���� ����Ͽ� �ֺ� �ȼ��� ���� ��ճ��� �� ó��
        float blurStrength = 0.9;  // �� ���� ����
        vec4 blurredColor = vec4(0.0);

        // �ֺ� 9�� �ȼ� ���ø� (����þ� ���� ũ�� ���� ����)
        float weight[9];
        weight[0] = 1.0 / 16.0; weight[1] = 2.0 / 16.0; weight[2] = 1.0 / 16.0;
        weight[3] = 2.0 / 16.0; weight[4] = 4.0 / 16.0; weight[5] = 2.0 / 16.0;
        weight[6] = 1.0 / 16.0; weight[7] = 2.0 / 16.0; weight[8] = 1.0 / 16.0;

        // �߾� ���� 8���� �ֺ� ���ø�
        blurredColor += texture(uTex2d, vTextureCoordinates + vec2(-texelSize.x, texelSize.y)) * weight[0];  // ���� ���
        blurredColor += texture(uTex2d, vTextureCoordinates + vec2(0.0, texelSize.y)) * weight[1];  // ��
        blurredColor += texture(uTex2d, vTextureCoordinates + vec2(texelSize.x, texelSize.y)) * weight[2];  // ������ ���

        blurredColor += texture(uTex2d, vTextureCoordinates + vec2(-texelSize.x, 0.0)) * weight[3];  // ����
        blurredColor += tex_color * weight[4];  // �߾�
        blurredColor += texture(uTex2d, vTextureCoordinates + vec2(texelSize.x, 0.0)) * weight[5];  // ������

        blurredColor += texture(uTex2d, vTextureCoordinates + vec2(-texelSize.x, -texelSize.y)) * weight[6];  // ���� �ϴ�
        blurredColor += texture(uTex2d, vTextureCoordinates + vec2(0.0, -texelSize.y)) * weight[7];  // �Ʒ�
        blurredColor += texture(uTex2d, vTextureCoordinates + vec2(texelSize.x, -texelSize.y)) * weight[8];  // ������ �ϴ�

        // ����þ� �� ���� ����
        tex_color.rgb = mix(tex_color.rgb, blurredColor.rgb, blurStrength);

        // Glow ȿ�� ����
        vec3 glowColor = vec3(1.0, 0.8, 0.5);  // ������ Glow ����
        float glowIntensity = edge * 1.5;  // ���ϰ� ����
        tex_color.rgb = mix(tex_color.rgb, glowColor, glowIntensity);
    }

    fFragClr = tex_color;
}
