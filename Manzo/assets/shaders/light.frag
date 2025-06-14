#version 330 core
in vec2 vScreenPos;
out vec4 FragColor;

uniform vec2 uLightPos;      // ship의 스크린 좌표
uniform float uLightRadius;  // 라이트 반지름
uniform sampler2D uObstacleMap;

void main() {
    // ship 위치를 기준으로 거리 계산
    float dist = distance(vScreenPos, uLightPos);
    
    // 라이트 반지름 밖은 검정색
    if(dist > uLightRadius) {
        FragColor = vec4(0.0, 0.0, 0.0, 1.0);
        return;
    }
    
    // 장애물 체크 (레이캐스팅)
    vec2 dir = normalize(vScreenPos - uLightPos);
    for(float t = 0.0; t < dist; t += 2.0) {
        vec2 samplePos = uLightPos + dir * t;
        vec2 uv = samplePos / vec2(textureSize(uObstacleMap, 0));
        if(texture(uObstacleMap, uv).r > 0.5) {
            FragColor = vec4(0.0, 0.0, 0.0, 1.0);
            return;
        }
    }
    
    // 라이트 효과
    float attenuation = 1.0 - (dist / uLightRadius);
    FragColor = vec4(1.0, 1.0, 1.0, 1.0); // 또는 원하는 라이트 색상
}
