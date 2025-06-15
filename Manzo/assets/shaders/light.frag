#version 330 core

in vec2 vWorldPos;
out vec4 FragColor;

uniform vec2 uLightPos;
uniform vec3 uLightColor;
uniform float uLightRadius;
uniform vec2 uScreenSize;
uniform sampler2D uObstacleMap;
uniform mat3 uWorldToScreen;

bool isObstacleAt(vec2 worldPos) {
    vec3 screenPos3 = uWorldToScreen * vec3(worldPos, 1.0);
    vec2 screenPos = screenPos3.xy; // screen 픽셀 위치
    vec2 uv = screenPos / uScreenSize;
    if(uv.x < 0.0 || uv.x > 1.0 || uv.y < 0.0 || uv.y > 1.0) return false; // 화면 밖 제외
    return texture(uObstacleMap, uv).r > 0.5;
}

void main() {
    vec2 fragPos = vWorldPos;
    vec2 dir = normalize(fragPos - uLightPos);
    float dist = length(fragPos - uLightPos);

    // 장애물에 가려진 픽셀 제거
    for (float t = 0.0; t < dist; t += 4.0) {
        vec2 samplePos = uLightPos + dir * t;
        if (isObstacleAt(samplePos))
        {
            FragColor = vec4(0.0, 0.0, 0.0, 1.0);
            return;
        }
    }

    // light 반경 밖은 discard 처리 (optional)
    if (dist > uLightRadius)
    {
        FragColor = vec4(0.0, 0.0, 0.0, 1.0);
        return;
     }

    // light 반경을 기준으로 alpha 부드럽게 마스킹 (smooth edge)
    // dist = 0 (center), uLightRadius = 최대반경
    float edgeWidth = 5.0; // 경계 부드러운 폭 (월드 단위)
    float alpha = smoothstep(uLightRadius, uLightRadius - edgeWidth, dist);

    // 빛 세기 감쇠
    float attenuation = 1.0 - (dist * dist) / (uLightRadius * uLightRadius);
    attenuation = clamp(attenuation, 0.0, 1.0);

    vec3 light = uLightColor * attenuation;

    FragColor = vec4(light, alpha);
}
