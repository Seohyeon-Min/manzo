#include "RayCasting.h"
#include "Camera.h"
#include <to_span.h>
#include "GameObjectManager.h"
#include "../Game/Fish.h"
#include "MathUtils.h"

Raycasting::Raycasting(GameObject* object_) : object(object_)
{
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);

	shader = Engine::GetShaderManager().GetShader("light");


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

GLuint Raycasting::createObstacleTexture(unsigned int width, unsigned int height, const std::list<GameObject*>& allObjects) {
    std::vector<unsigned char> data(width * height, 0);

    for (auto* obj : allObjects) {

        int startX = (int)obj->GetPosition().x;
        int startY = (int)obj->GetPosition().y;

        auto sprite = obj->GetGOComponent<Sprite>();
        if (!sprite) continue;  // Sprite ¾øÀ¸¸é ½ºÅµ

        int texWidth = sprite->GetTexture()->GetWidth();
        int texHeight = sprite->GetTexture()->GetHeight();

        for (int y = startY; y < startY + texHeight && y < (int)height; ++y) {
            for (int x = startX; x < startX + texWidth && x < (int)width; ++x) {
                if (x >= 0 && y >= 0)
                    data[y * width + x] = 255;
            }
        }
    }

    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data.data());

    return tex;
}


void Raycasting::Update(double dt)
{
   // obstacleTex = createObstacleTexture(Engine::window_width, Engine::window_height, Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->GetAllObjects());

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_DST_COLOR, GL_ZERO);

    Cam* cam = Engine::GetGameStateManager().GetGSComponent<Cam>();
    shader->Use();

    ///////////////////////////////////////////////////////////////////////////////////
    UpdateRadius();

    float w = (float)Engine::window_width;
    float h = (float)Engine::window_height;
    vec2 shipWorldPos = object->GetPosition();
    mat3 modelToWorld = mat3::build_translation(shipWorldPos) *
        mat3::build_scale(radius, radius);
    shader->SendUniform("uModelToWorld", util::to_span(modelToWorld));
    mat3 WORLD_TO_NDC = cam->world_to_ndc;
    mat3 modelToNDC = WORLD_TO_NDC * modelToWorld;

    mat3 ndc_to_screen = mat3::build_translation({ w / 2.f, h / 2.f }) * mat3::build_scale({ w / 2.f, h / 2.f });

    mat3 world_to_screen = ndc_to_screen * WORLD_TO_NDC;
    mat3 screenToWorld = Inverse(world_to_screen);

    shader->SendUniform("uScreenToWorld", util::to_span(screenToWorld));
    shader->SendUniform("uModelToNDC", util::to_span(modelToNDC));
    shader->SendUniform("uWorldToScreen", util::to_span(world_to_screen));
    ///////////////////////////////////////////////////////////////////////////////////

    shader->SendUniform("uLightPos", shipWorldPos.x, shipWorldPos.y);
    shader->SendUniform("uLightColor", 1.0f, 0.9f, 0.7f);

    ///////////////////////////////////////////////////////////////////////////////////

	shader->SendUniform("uLightRadius", radius);

    ///////////////////////////////////////////////////////////////////////////////////


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