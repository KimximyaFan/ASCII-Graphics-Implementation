#include <memory>
#include "scene/entity.h"
#include "geometry/mesh.h"
#include "material/material.h"
#include "math/vector.h"

// 테스트용 큐브 Entity 생성
std::shared_ptr<Entity> CreateTestCubeEntity(float size = 1.0f)
{
    float hs = size * 0.5f;  // half size

    // 1) 정점(Vertex) 정의
    // Vertex 구조체가 position:Vec3, normal:Vec3, color:Color 라고 가정
    std::vector<Vertex> vertices = {
        // 앞면 (–Z)
        {{-hs, -hs, -hs, 1.0f}, { 0,  0, -1}, {1,1,1,1}},
        {{ hs, -hs, -hs, 1.0f}, { 0,  0, -1}, {1,1,1,1}},
        {{ hs,  hs, -hs, 1.0f}, { 0,  0, -1}, {1,1,1,1}},
        {{-hs,  hs, -hs, 1.0f}, { 0,  0, -1}, {1,1,1,1}},
        // 뒷면 (+Z)
        {{-hs, -hs,  hs, 1.0f}, { 0,  0,  1}, {1,1,1,1}},
        {{ hs, -hs,  hs, 1.0f}, { 0,  0,  1}, {1,1,1,1}},
        {{ hs,  hs,  hs, 1.0f}, { 0,  0,  1}, {1,1,1,1}},
        {{-hs,  hs,  hs, 1.0f}, { 0,  0,  1}, {1,1,1,1}},
        // 왼쪽 (–X)
        {{-hs, -hs, -hs, 1.0f}, {-1,  0,  0}, {1,1,1,1}},
        {{-hs,  hs, -hs, 1.0f}, {-1,  0,  0}, {1,1,1,1}},
        {{-hs,  hs,  hs, 1.0f}, {-1,  0,  0}, {1,1,1,1}},
        {{-hs, -hs,  hs, 1.0f}, {-1,  0,  0}, {1,1,1,1}},
        // 오른쪽 (+X)
        {{ hs, -hs, -hs, 1.0f}, { 1,  0,  0}, {1,1,1,1}},
        {{ hs,  hs, -hs, 1.0f}, { 1,  0,  0}, {1,1,1,1}},
        {{ hs,  hs,  hs, 1.0f}, { 1,  0,  0}, {1,1,1,1}},
        {{ hs, -hs,  hs, 1.0f}, { 1,  0,  0}, {1,1,1,1}},
        // 아래 (–Y)
        {{-hs, -hs, -hs, 1.0f}, { 0, -1,  0}, {1,1,1,1}},
        {{-hs, -hs,  hs, 1.0f}, { 0, -1,  0}, {1,1,1,1}},
        {{ hs, -hs,  hs, 1.0f}, { 0, -1,  0}, {1,1,1,1}},
        {{ hs, -hs, -hs, 1.0f}, { 0, -1,  0}, {1,1,1,1}},
        // 위 (+Y)
        {{-hs,  hs, -hs, 1.0f}, { 0,  1,  0}, {1,1,1,1}},
        {{-hs,  hs,  hs, 1.0f}, { 0,  1,  0}, {1,1,1,1}},
        {{ hs,  hs,  hs, 1.0f}, { 0,  1,  0}, {1,1,1,1}},
        {{ hs,  hs, -hs, 1.0f}, { 0,  1,  0}, {1,1,1,1}},
    };

    // 2) 인덱스 정의 (24개 정점을 12개의 삼각형으로)
    std::vector<uint32_t> indices = {
        0,1,2,  2,3,0,        // 앞
        4,6,5,  6,4,7,        // 뒤
        8,9,10, 10,11,8,      // 왼쪽
        12,14,13, 14,12,15,   // 오른쪽
        16,17,18, 18,19,16,   // 아래
        20,22,21, 22,20,23    // 위
    };

    // 3) Mesh 생성
    Mesh cubeMesh;
    cubeMesh.vertices = std::move(vertices);
    cubeMesh.indices = std::move(indices);

    // 4) 테스트용 머티리얼
    auto mat = std::make_shared<Material>();
    mat->ambient_coef    = 0.2f;
    mat->diffuse_coef    = 1.0f;
    mat->specular_coef   = 0.5f;
    mat->shininess       = 16.0f;
    cubeMesh.material = mat;

    // 5) Entity에 메시 추가
    auto entity = std::make_shared<Entity>();
    entity->parts.push_back(cubeMesh);

    entity->ComputeLocalAABB();

    return entity;
}

// 사용 예:
// auto cubeEntity = CreateTestCubeEntity(2.0f);
// scene.AddEntity(cubeEntity);
