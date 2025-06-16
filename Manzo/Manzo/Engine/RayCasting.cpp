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
    shader->SendUniform("uScreenSize", w, h);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // 초기 빈 데이터 (동적업데이트 예정)
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * (NUM_RAYS + 2), nullptr, GL_DYNAMIC_DRAW);

    // 위치 attribute (vec2)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    glBindVertexArray(0);
}

Raycasting::~Raycasting()
{
    glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	shader = nullptr;
}

#include <vector>
#include <cmath>
#include "MathUtils.h"

void Raycasting::UpdateMesh()
{
    vec2 lightPos = object->GetPosition();

    // cam, world_to_ndc, ndc_to_screen 등을 얻는다
    Cam* cam = Engine::GetGameStateManager().GetGSComponent<Cam>();
    mat3 WORLD_TO_NDC = cam->world_to_ndc;
    mat3 ndc_to_screen = mat3::build_translation({ w / 2.f, h / 2.f }) * mat3::build_scale({ w / 2.f, h / 2.f });
    mat3 world_to_screen = ndc_to_screen * WORLD_TO_NDC;

    std::vector<vec2> points;
    points.reserve(NUM_RAYS);

    // raycast 로 obstacle 검사 (world 좌표계)
    for (int i = 0; i < NUM_RAYS; ++i)
    {
        float angle = (float)i / NUM_RAYS * 2.0f * (float)M_PI;
        vec2 dir = { cos(angle), sin(angle) };

        float stepSize = 5.0f;
        float maxDist = radius;

        vec2 pos = lightPos;
        float dist = 0.0f;
        bool hit = false;

        // 장애물 맵 접근을 위한 람다 함수
        auto isObstacleAt = [&](vec2 worldPos) -> bool {
            // 월드 좌표를 장애물 맵 좌표로 변환
            vec2 cameraPos = Engine::GetGameStateManager().GetGSComponent<Cam>()->GetTargetPosition();
            float camViewWidth = 20.0f;   // 카메라 뷰 폭
            float camViewHeight = 15.0f;  // 카메라 뷰 높이

            float world_left = cameraPos.x - camViewWidth / 2.0f;
            float world_top = cameraPos.y + camViewHeight / 2.0f;
            float world_width = camViewWidth;
            float world_height = camViewHeight;

            int maskX = static_cast<int>(((worldPos.x - world_left) / world_width) * 1280);
            int maskY = static_cast<int>(((world_top - worldPos.y) / world_height) * 720);

            // 범위 체크
            if (maskX < 0 || maskX >= 1280 || maskY < 0 || maskY >= 720)
                return false;

            // 장애물 맵 데이터 가져오기
            auto obstacleMap = Engine::GetGameStateManager().GetGSComponent<MapManager>()->GetMap(0);

            std::fill(obstacleMap->data.begin(), obstacleMap->data.end(), 0);
            // IsObstacleAt 호출 (클래스 멤버 함수)
            return IsObstacleAt(maskX, maskY, 1280, 720, obstacleMap->data);
            };

        while (dist < maxDist)
        {
            pos = pos + dir * stepSize;
            dist += stepSize;

            // 장애물 판정
            if (isObstacleAt(pos)) {
                hit = true;
                break;
            }
        }

        // world 좌표 pos → screen 좌표로 변환
        vec3 p_world = { pos.x, pos.y, 1.0f };
        vec3 p_screen = world_to_screen * p_world;
        points.push_back({ p_screen.x, p_screen.y });
    }


    // 빛 위치도 screen 좌표로 변환
    vec3 lightPos_screen = world_to_screen * vec3{ lightPos.x, lightPos.y, 1.0f };

    // 삼각형 팬용 버텍스 배열 생성
    std::vector<float> vertices;
    vertices.reserve((NUM_RAYS + 2) * 2);

    vertices.push_back(lightPos_screen.x);
    vertices.push_back(lightPos_screen.y);

    for (auto& p : points)
    {
        vertices.push_back(p.x);
        vertices.push_back(p.y);
    }

    vertices.push_back(points[0].x);
    vertices.push_back(points[0].y);

    numVertices = NUM_RAYS + 2;

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float), vertices.data());
}

void Raycasting::Render()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_DST_COLOR, GL_ZERO);

    UpdateMesh();

    Cam* cam = Engine::GetGameStateManager().GetGSComponent<Cam>();
    shader->Use();

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, Engine::GetGameStateManager().GetGSComponent<MapManager>()->GetMap(0)->obstacleTex);  // map에서 가져옴

    shader->SendUniform("uObstacleMap", 1);

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
    glDrawArrays(GL_TRIANGLE_FAN, 0, numVertices);
    glBindVertexArray(0);

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