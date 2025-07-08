#pragma once

#include <memory>
#include "scene/entity.h"
#include "geometry/mesh.h"
#include "material/material.h"
#include "math/vector.h"
#include <iostream>

void ComputeVertexNormals(Mesh& mesh) {
    // 1) 정점 법선 초기화
    for (auto& v : mesh.vertices) {
        v.normal = Vec3{0.0f, 0.0f, 0.0f};
    }

    // 2) 각 삼각형에 대해 면 법선 계산 후, 해당 정점들에 누적
    for (size_t i = 0; i + 2 < mesh.indices.size(); i += 3) {
        unsigned int i0 = mesh.indices[i + 0];
        unsigned int i1 = mesh.indices[i + 1];
        unsigned int i2 = mesh.indices[i + 2];

        Vec3 p0 = mesh.vertices[i0].position.ToVec3();
        Vec3 p1 = mesh.vertices[i1].position.ToVec3();
        Vec3 p2 = mesh.vertices[i2].position.ToVec3();

        Vec3 edge1 = p1 - p0;
        Vec3 edge2 = p2 - p0;
        Vec3 faceNormal = Vec3::Normalize(Vec3::Cross(edge1, edge2));

        mesh.vertices[i0].normal += faceNormal;
        mesh.vertices[i1].normal += faceNormal;
        mesh.vertices[i2].normal += faceNormal;
    }

    // 3) 누적된 법선을 정규화하여 최종 정점 법선으로 사용
    /*
    for (auto& v : mesh.vertices) {
        v.normal = Vec3::Normalize(v.normal);
        printf("%.2f %.2f %.2f\n", v.normal.x, v.normal.y, v.normal.z);
    }
        */
}

// 테스트용 큐브 Entity 생성
std::shared_ptr<Entity> CreateTestCubeEntity(float size = 1.0f)
{
    float hs = size * 0.5f;  // half size

    // 1) 정점(Vertex) 정의
    // Vertex 구조체가 position:Vec3, normal:Vec3, color:Color 라고 가정
    std::vector<Vertex> vertices = {
        {{-hs,-hs,-hs,1}, {}, {0.5, 0.5, 0.5, 0}},  // 0
        {{ hs,-hs,-hs,1}, {}, {0.5, 0.5, 0.5, 0}},  // 1
        {{ hs, hs,-hs,1}, {}, {0.5, 0.5, 0.5, 0}},  // 2
        {{-hs, hs,-hs,1}, {}, {0.5, 0.5, 0.5, 0}},  // 3
        {{-hs,-hs, hs,1}, {}, {0.5, 0.5, 0.5, 0}},  // 4
        {{ hs,-hs, hs,1}, {}, {0.5, 0.5, 0.5, 0}},  // 5
        {{ hs, hs, hs,1}, {}, {0.5, 0.5, 0.5, 0}},  // 6
        {{-hs, hs, hs,1}, {}, {0.5, 0.5, 0.5, 0}},  // 7
    };

    // 2) 삼각형 인덱스 (12삼각형)
    std::vector<uint32_t> indices = {
        0,1,2,  0,2,3,  // 앞면
        5,4,7,  5,7,6,  // 뒷면
        4,0,3,  4,3,7,  // 왼쪽
        1,5,6,  1,6,2,  // 오른쪽
        4,5,1,  4,1,0,  // 아래
        3,2,6,  3,6,7,  // 위
    };

    // 3) Mesh 생성
    Mesh cubeMesh;
    cubeMesh.vertices = std::move(vertices);
    cubeMesh.indices = std::move(indices);

    ComputeVertexNormals(cubeMesh);

    // 4) 테스트용 머티리얼
    auto mat = std::make_shared<Material>();
    mat->ambient_coef    = 1.0f;
    mat->diffuse_coef    = 1.0f;
    mat->specular_coef   = 0.01f;
    mat->shininess       = 0.0f;
    cubeMesh.material = mat;

    // 5) Entity에 메시 추가
    auto entity = std::make_shared<Entity>();
    entity->parts.push_back(cubeMesh);
    entity->transform.position = Vec3(0, 3.0f, 0);
    entity->transform.scale = Vec3(1.0f, 0.4f, 1.0f);

    entity->ComputeLocalAABB();

    return entity;
}

// 사용 예:
// auto cubeEntity = CreateTestCubeEntity(2.0f);
// scene.AddEntity(cubeEntity);
