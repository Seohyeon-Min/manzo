#version 450 core

// input
layout(location = 0) in vec3 vColor;
layout(location = 1) in vec2 vTextureCoordinates;

// uniform variable
uniform sampler2D uTexture;   // texture of obj
uniform vec2 uResolution;     // resolution
uniform vec2 uBlurDirection;  // direction of blur

// output var
out vec4 FragColor;

// gaussian 
const float weights[5] = float[](0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main() {
    vec2 texelSize = 1.0 / uResolution; // calculate size of texel
    vec3 color = texture(uTexture, vTextureCoordinates).rgb * weights[0]; // center pixel

    // bluring
    for (int i = 1; i < 5; ++i) {
        vec2 offset = uBlurDirection * float(i) * texelSize; // texel offset
        color += texture(uTexture, vTextureCoordinates + offset).rgb * weights[i];
        color += texture(uTexture, vTextureCoordinates - offset).rgb * weights[i];
    }

    FragColor = vec4(color, 1.0);
}