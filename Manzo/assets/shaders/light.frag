#version 330 core
in vec2 vScreenPos;
out vec4 FragColor;

uniform vec2 uLightPos;      // ship�� ��ũ�� ��ǥ
uniform float uLightRadius;  // ����Ʈ ������
uniform sampler2D uObstacleMap;

void main() {
    // ship ��ġ�� �������� �Ÿ� ���
    float dist = distance(vScreenPos, uLightPos);
    
    // ����Ʈ ������ ���� ������
    if(dist > uLightRadius) {
        FragColor = vec4(0.0, 0.0, 0.0, 1.0);
        return;
    }
    
    // ��ֹ� üũ (����ĳ����)
    vec2 dir = normalize(vScreenPos - uLightPos);
    for(float t = 0.0; t < dist; t += 2.0) {
        vec2 samplePos = uLightPos + dir * t;
        vec2 uv = samplePos / vec2(textureSize(uObstacleMap, 0));
        if(texture(uObstacleMap, uv).r > 0.5) {
            FragColor = vec4(0.0, 0.0, 0.0, 1.0);
            return;
        }
    }
    
    // ����Ʈ ȿ��
    float attenuation = 1.0 - (dist / uLightRadius);
    FragColor = vec4(1.0, 1.0, 1.0, 1.0); // �Ǵ� ���ϴ� ����Ʈ ����
}
