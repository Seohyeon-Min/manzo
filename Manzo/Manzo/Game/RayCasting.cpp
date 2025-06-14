#include "RayCasting.h"

GLuint Raycasting::createObstacleTexture(unsigned int width, unsigned int height) {
    std::vector<unsigned char> data(width * height, 0);

    // Create a simple box obstacle in the center
    for (unsigned int y = 550; y < 750; ++y) {
        for (unsigned int x = 350; x < 450; ++x) {
            data[y * width + x] = 255; // Mark as obstacle
        }
    }

    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    return tex;
}

Raycasting::Raycasting(GameObject* object_) : object(object_)
{
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);

	shader = Engine::GetShaderManager().GetShader("light");
    obstacleTex = createObstacleTexture(Engine::window_width, Engine::window_height);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    shader->Use();
    shader->SendUniform("uObstacleMap", 0);
    shader->SendUniform("uScreenSize", Engine::window_width, Engine::window_height);

    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);

}

Raycasting::~Raycasting()
{
    glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	shader = nullptr;
}

void Raycasting::Update(double dt)
{
    shader->Use();

    std::cout << "Mouse Pos: " << Engine::GetInput().GetMousePos().mouseCamSpaceX + Engine::window_width / 2.f << ", " << Engine::GetInput().GetMousePos().mouseCamSpaceY + Engine::window_height / 2.f << std::endl;
    std::cout << "Ship World Pos: " << object->GetPosition().x << ", " << object->GetPosition().y << std::endl;

    
    shader->SendUniform("uLightPos", Engine::GetInput().GetMousePos().mouseCamSpaceX + Engine::window_width / 2.f, Engine::GetInput().GetMousePos().mouseCamSpaceY + Engine::window_height / 2.f);
	shader->SendUniform("uLightColor", 1.0f, 0.9f, 0.7f);
	shader->SendUniform("uLightRadius", 250.0f);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, obstacleTex);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
