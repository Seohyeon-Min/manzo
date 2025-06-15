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
    shader->SendUniform("uScreenSize", w, h);

}

Raycasting::~Raycasting()
{
    glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	shader = nullptr;
}

void Raycasting::Render()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_DST_COLOR, GL_ZERO);

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

    mat3 ndc_to_screen = mat3::build_translation({ w / 2.f, h / 2.f }) * mat3::build_scale({ w / 2.f, h / 2.f });

    mat3 world_to_screen = ndc_to_screen * WORLD_TO_NDC;
    mat3 screenToWorld = Inverse(world_to_screen);

    shader->SendUniform("uScreenToWorld", util::to_span(screenToWorld));
    shader->SendUniform("uModelToNDC", util::to_span(modelToNDC));
    shader->SendUniform("uWorldToScreen", util::to_span(world_to_screen));
    ///////////////////////////////////////////////////////////////////////////////////

    shader->SendUniform("uLightPos", shipWorldPos.x, shipWorldPos.y);
    shader->SendUniform("uLightColor", 1.0f, 1.0f, 1.0f);

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

    if (y < minY) y = minY;
    if (y > maxY) y = maxY;

    float t = (y - minY) / (maxY - minY);

    radius = minRadius + t * (maxRadius - minRadius);
}