#version 450 core

out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D uSceneTexture;  // texture
uniform float uThreshold;         // threshold about light
uniform vec2 uBlurDirection;      // blur direction
uniform float uResolution;        // texture resolution
uniform float uBloomIntensity;    // weight of bloom

void main() {
    vec3 sceneColor = texture(uSceneTexture, TexCoords).rgb;

    vec3 expandedBrightColor = vec3(0.0); // clear
    int radius = 5;                       // radius
    float weightSum = 0.0;                 // weight sum
    float stepSize = 1.0 / uResolution;   // texel size

    for (int x = -radius; x <= radius; x++) {
        for (int y = -radius; y <= radius; y++) {
            vec2 offset = vec2(x, y) * stepSize;
            float distance = length(vec2(x, y));    // direction to this
            if (distance <= float(radius)) {        // if it's in radius
                float weight = 1.0 - (distance / float(radius));    // weight depends on distance
                vec3 neighborColor = texture(uSceneTexture, TexCoords + offset).rgb;
                float neighborBrightness = dot(neighborColor, vec3(0.3, 0.59, 0.11)) * 0.7;                             // maybe brightness?
                float neighborSoftBrightness = smoothstep(uThreshold - 0.2, uThreshold + 0.04, neighborBrightness);
                vec3 neighborBrightColor = neighborSoftBrightness * neighborColor;
                expandedBrightColor += neighborBrightColor * weight;
                weightSum += weight;
            }
        }
    }

    if (weightSum > 0.0) {
        expandedBrightColor /= weightSum; // normalize
    }

    vec3 result = sceneColor + expandedBrightColor * uBloomIntensity;
    FragColor = vec4(result, 1.0); // output
}
