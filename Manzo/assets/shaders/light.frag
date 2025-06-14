#version 330 core

in vec2 vWorldPos;
out vec4 FragColor;

uniform vec2 uLightPos;
uniform vec3 uLightColor;
uniform float uLightRadius;
uniform vec2 uScreenSize;
uniform sampler2D uObstacleMap;

bool isObstacleAt(vec2 screenPos) {
    vec2 uv = screenPos / uScreenSize;
    return texture(uObstacleMap, uv).r > 0.5;
}


void main() {
    vec2 fragPos = vWorldPos;
    vec2 dir = normalize(fragPos - uLightPos);
    float dist = length(fragPos - uLightPos);

    if (dist > uLightRadius) discard;

    for (float t = 0.0; t < dist; t += 4.0) {
        vec2 samplePos = uLightPos + dir * t;
        if (isObstacleAt(samplePos)) discard;
    }

    float attenuation = 1.0 - dist / uLightRadius;
    vec3 light = uLightColor * attenuation;
    FragColor = vec4(light, 1.0);
}