#include "RayCasting.h"
#include "Camera.h"
#include <to_span.h>
#include "GameObjectManager.h"

void Raycasting::CreateObstacleFBO(int width, int height)
{
    glGenFramebuffers(1, &obstacleFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, obstacleFBO);

    glGenTextures(1, &obstacleTexture);
    glBindTexture(GL_TEXTURE_2D, obstacleTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, obstacleTexture, 0);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "Obstacle FBO is not complete!" << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


Raycasting::Raycasting(GameObject* object_) : object(object_)
{
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);

	shader = Engine::GetShaderManager().GetShader("light");
    //obstacleTex = createObstacleTexture(Engine::window_width, Engine::window_height);


    shader->Use();
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
    RenderObstacleMap(Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->GetAllObjects());


    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_DST_ALPHA);

    shader = Engine::GetShaderManager().GetShader("light");
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

    mat3 worldToNDC = cam->world_to_ndc;
    shader->SendUniform("uWorldToNDC", util::to_span(worldToNDC));

    ///////////////////////////////////////////////////////////////////////////////////

    shader->SendUniform("uLightPos", shipWorldPos.x, shipWorldPos.y);
    shader->SendUniform("uLightColor", 1.0f, 0.9f, 0.7f);

    ///////////////////////////////////////////////////////////////////////////////////

	shader->SendUniform("uLightRadius", radius);

    ///////////////////////////////////////////////////////////////////////////////////
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, obstacleTexture);
    shader->SendUniform("uObstacleMap", 1);

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

#include "Sprite.h"
void Raycasting::RenderObstacleMap(std::list<GameObject*> gameObjects)
{
    if (!obstacleFBO) {
        CreateObstacleFBO(Engine::window_width, Engine::window_height);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, obstacleFBO);
    glViewport(0, 0, Engine::window_width, Engine::window_height);
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    GLShader* obstacleShader = Engine::GetShaderManager().GetShader("obstacle_pass");
    obstacleShader->Use();

    // 카메라 시야 영역 계산
    Cam* cam = Engine::GetGameStateManager().GetGSComponent<Cam>();
    vec2 cameraPos = cam->GetPosition();
    float viewWidth = Engine::window_width / cam->GetCameraView().GetZoom();
    float viewHeight = Engine::window_height / cam->GetCameraView().GetZoom();

    // 프러스텀 경계 계산
    float left = cameraPos.x - viewWidth * 0.5f;
    float right = cameraPos.x + viewWidth * 0.5f;
    float top = cameraPos.y + viewHeight * 0.5f;
    float bottom = cameraPos.y - viewHeight * 0.5f;

    for (GameObject* obj : gameObjects) {
        Sprite* sprite = obj->GetGOComponent<Sprite>();
        if (!sprite) continue;

        // 화면 내 오브젝트 필터링
        vec2 objPos = obj->GetPosition();
        float objSize = std::max(sprite->GetTexture()->GetWidth(),
            sprite->GetTexture()->GetHeight()) * 0.5f;

        if (objPos.x + objSize < left || objPos.x - objSize > right ||
            objPos.y + objSize < bottom || objPos.y - objSize > top) {
            continue; // 화면 밖 오브젝트 스킵
        }

        // 화면 내 오브젝트만 렌더링
        mat3 modelToWorld = obj->GetMatrix();
        mat3 modelToNDC = cam->world_to_ndc * modelToWorld;

        obstacleShader->SendUniform("uModelToNDC", util::to_span(modelToNDC));
        obstacleShader->SendUniform("uUV", 0.f, 0.f, 1.f, 1.f);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, sprite->GetTexture()->GetHandle());
        obstacleShader->SendUniform("uTex2d", 0);

        Engine::GetRender().RenderQuad();
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
