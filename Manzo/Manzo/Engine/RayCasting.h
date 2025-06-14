#pragma once

#include "GameObject.h"
#include "ShaderManager.h"
#include <ext/matrix_float4x4.hpp>

class Raycasting {
public:
    GLuint createObstacleTexture(unsigned int width, unsigned int height);
    Raycasting(GameObject* object_);
    ~Raycasting();
    void Update(double dt);
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
};