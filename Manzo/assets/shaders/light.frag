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
    uv.x = 1.0 - uv.x;
    if(uv.x < 0.0 || uv.x > 1.0 || uv.y < 0.0 || uv.y > 1.0) return false; // 화면 밖 제외
    return texture(uObstacleMap, uv).r > 0.1;
}

void main() {
    vec2 fragPos = vWorldPos;
    vec2 dir = normalize(-fragPos + uLightPos);
    float dist = length(fragPos - uLightPos);
    
    if(dist > uLightRadius) {
    FragColor = vec4(0.0, 0.0, 0.0, 1.0); // 빛 범위 밖은 투명
		return;
	}

    
    float edgeWidth = 15.0; 
    float alpha = smoothstep(uLightRadius, uLightRadius - edgeWidth, dist);

    float attenuation = 1.0 - (dist * dist) / (uLightRadius * uLightRadius);
    attenuation = clamp(attenuation, 0.0, 1.0);

    vec3 light = uLightColor * attenuation;

    for (float t = 0.0; t < dist; t += 1.0) {
        vec2 samplePos = uLightPos + dir * t;
        if (isObstacleAt(samplePos))
        {
            light = vec3(0.0, 0.0, 0.0) * attenuation;
        }
    }


    FragColor = vec4(light, alpha);
}
