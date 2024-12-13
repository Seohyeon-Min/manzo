#version 450 core

out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D uSceneTexture;
uniform float uThreshold; 

void main() {
    vec3 color = texture(uSceneTexture, TexCoords).rgb;
    float brightness = dot(color, vec3(0.2126, 0.7152, 0.0722)); // (Luminance)
    if (brightness > uThreshold) {
        FragColor = vec4(color, 1.0); // remain light area
    } else {
        FragColor = vec4(0.0); // remov darkness
    }
}