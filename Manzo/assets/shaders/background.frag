#version 330 core
out vec4 FragColor;

uniform vec3 uBackgroundColor; // RGB

void main() {
    FragColor = vec4(uBackgroundColor, 1.0);
}
