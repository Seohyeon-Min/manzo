#pragma once

#include "GameObject.h"
#include "ShaderManager.h"
#include <ext/matrix_float4x4.hpp>

class Raycasting {
public:

    void CreateObstacleFBO(int width, int height);
    Raycasting(GameObject* object_);
    ~Raycasting();
    void Update(double dt);
    void UpdateRadius();
    void RenderObstacleMap(std::list<GameObject*> gameObjects);

private:
    GameObject* object;
    GLShader* shader;

    float quadVertices[24] = {
        // positions    // texCoords
        -1.0f, -1.0f,   0.0f, 0.0f,
         1.0f, -1.0f,   1.0f, 0.0f,
         1.0f,  1.0f,   1.0f, 1.0f,

        -1.0f, -1.0f,   0.0f, 0.0f,
         1.0f,  1.0f,   1.0f, 1.0f,
        -1.0f,  1.0f,   0.0f, 1.0f
    };
    GLuint obstacleFBO, obstacleTexture;
    GLuint VAO, VBO;
    float radius;
};