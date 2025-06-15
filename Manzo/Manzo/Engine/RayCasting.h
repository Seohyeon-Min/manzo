#pragma once

#include "GameObject.h"
#include "ShaderManager.h"
#include <ext/matrix_float4x4.hpp>

class Raycasting {
public:
    Raycasting(GameObject* object_);
    ~Raycasting();
    void Render();
    void UpdateRadius();

private:
    GameObject* object;
    GLShader* shader;
	GLuint obstacleTex;

    float quadVertices[24] = {
        // positions    // texCoords
        -1.0f, -1.0f,   0.0f, 0.0f,
         1.0f, -1.0f,   1.0f, 0.0f,
         1.0f,  1.0f,   1.0f, 1.0f,

        -1.0f, -1.0f,   0.0f, 0.0f,
         1.0f,  1.0f,   1.0f, 1.0f,
        -1.0f,  1.0f,   0.0f, 1.0f
    };

    GLuint VAO, VBO;
    float radius;


    float minY = -5000.f;
    float maxY = 0.f;

    float minRadius = 100.f;
    float maxRadius = 1000.f;


    float w = (float)Engine::window_width;
    float h = (float)Engine::window_height;
};