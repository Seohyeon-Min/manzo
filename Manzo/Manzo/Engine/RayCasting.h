#pragma once

#include "GameObject.h"
#include "ShaderManager.h"
#include <ext/matrix_float4x4.hpp>

class Raycasting {
public:
    Raycasting(GameObject* object_);
    ~Raycasting();
    void UpdateMesh();
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

    int numVertices = 0;  // ���� ���ؽ� ���� (�ﰢ�� ������ �������� �� ����)
    const int NUM_RAYS = 30;  // ray ����

    float minY = -5000.f;
    float maxY = 0.f;

    float minRadius = 100.f;
    float maxRadius = 1000.f;


    float w = (float)Engine::window_width;
    float h = (float)Engine::window_height;

    std::vector<vec2> ray_hit_points;

    // ��ֹ� �� �����Ϳ� ũ�⸦ �ܺο��� �����ϰų� ĸó
    bool IsObstacleAt(int x, int y, int mapWidth, int mapHeight, const std::vector<unsigned char>& data) {
        if (x < 0 || x >= mapWidth || y < 0 || y >= mapHeight) return false;
        int flippedY = mapHeight - 1 - y; // y�� �ø�
        int index = flippedY * mapWidth + x;
        return data[index] > 127; // 127 �ʰ��� ��ֹ�
    }
};