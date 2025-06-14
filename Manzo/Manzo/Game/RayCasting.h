#pragma once

#include "../Engine/Component.h"
#include "../Engine/GameObject.h"
#include "../Engine/ShaderManager.h"
#include <ext/matrix_float4x4.hpp>

class Raycasting : public Component {
public:
    GLuint createObstacleTexture(unsigned int width, unsigned int height);
    Raycasting(GameObject* object_);
    ~Raycasting();
    void Update(double dt) override;

private:
    GameObject* object;
    GLShader* shader;
	GLuint obstacleTex;

    float quadVertices[24] = {
    -1.0f,  1.0f,  0.0f, 1.0f,
    -1.0f, -1.0f,  0.0f, 0.0f,
     1.0f, -1.0f,  1.0f, 0.0f,

    -1.0f,  1.0f,  0.0f, 1.0f,
     1.0f, -1.0f,  1.0f, 0.0f,
     1.0f,  1.0f,  1.0f, 1.0f
    };

    GLuint VAO, VBO;
};