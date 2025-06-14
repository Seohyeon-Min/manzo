#include "RayCasting.h"
#include "Camera.h"
#include <to_span.h>

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
    //obstacleTex = createObstacleTexture(Engine::window_width, Engine::window_height);


    shader->Use();
    shader->SendUniform("uObstacleMap", 1);
    shader->SendUniform("uScreenSize", Engine::window_width, Engine::window_height);

}

Raycasting::~Raycasting()
{
    glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	shader = nullptr;
}

void Raycasting::Update(double dt)
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_DST_COLOR);

    Cam* cam = Engine::GetGameStateManager().GetGSComponent<Cam>();
    shader->Use();

    ///////////////////////////////////////////////////////////////////////////////////
    UpdateRadius();

    vec2 shipWorldPos = object->GetPosition();
    mat3 modelToWorld = mat3::build_translation(shipWorldPos) *
        mat3::build_scale(radius, radius);
    shader->SendUniform("uModelToWorld", util::to_span(modelToWorld));
    mat3 WORLD_TO_NDC = cam->world_to_ndc;
    mat3 modelToNDC = WORLD_TO_NDC * modelToWorld;
    shader->SendUniform("uModelToNDC", util::to_span(modelToNDC));
    ///////////////////////////////////////////////////////////////////////////////////

    shader->SendUniform("uLightPos", shipWorldPos.x, shipWorldPos.y);
    shader->SendUniform("uLightColor", 1.0f, 0.9f, 0.7f);

    ///////////////////////////////////////////////////////////////////////////////////

	shader->SendUniform("uLightRadius", radius);

    ///////////////////////////////////////////////////////////////////////////////////
    //glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D, obstacleTex);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

     Engine::GetRender().RenderQuad();
}

void Raycasting::UpdateRadius()
{
    float y = object->GetPosition().y;

    float minY = -6000.f;
    float maxY = 0.f;

    float minRadius = 50.f;
    float maxRadius = 300.f;

    if (y < minY) y = minY;
    if (y > maxY) y = maxY;

    float t = (y - minY) / (maxY - minY);

    radius = minRadius + t * (maxRadius - minRadius);
}