#version 450 core

layout (location = 0) in vec2 vertexPosition;
layout (location = 1) in vec2 vertexUV;
layout (location = 2) in int  vertexIndex;

out vec2 uv;
flat out int bufferIndex;

void main() {
	gl_Position = vec4(vertexPosition, 0, 1);
	uv = vertexUV;
	bufferIndex = vertexIndex;
}
