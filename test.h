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



// Vertex: { position: Vec4, normal: Vec3, color: Color } 라고 가정
// position.w = 1.0f

std::shared_ptr<Entity> CreateCubeEntity_Flat24(float size = 1.0f)
{
    float hs = size * 0.5f; // half size
    const Color faceColor = {0.5f, 0.5f, 0.5f, 0.0f};

    auto V4 = [&](float x, float y, float z) { return Vec4{x, y, z, 1.0f}; };
    auto N3 = [&](float x, float y, float z) { return Vec3{x, y, z}; };

    std::vector<Vertex> vertices;
    vertices.reserve(24);
    std::vector<uint32_t> indices;
    indices.reserve(36);

    auto addFace = [&](Vec4 a, Vec4 b, Vec4 c, Vec4 d, Vec3 n) {
        // a,b,c,d: 바깥에서 보았을 때 CCW 순서
        uint32_t base = static_cast<uint32_t>(vertices.size());
        vertices.push_back({a, n, faceColor}); // 0
        vertices.push_back({b, n, faceColor}); // 1
        vertices.push_back({c, n, faceColor}); // 2
        vertices.push_back({d, n, faceColor}); // 3
        // 두 개의 삼각형
        indices.push_back(base + 0);
        indices.push_back(base + 1);
        indices.push_back(base + 2);
        indices.push_back(base + 0);
        indices.push_back(base + 2);
        indices.push_back(base + 3);
    };

    // 8 모서리 좌표 (이름은 비트 느낌으로 xyz 부호)
    Vec4 v000 = V4(-hs, -hs, -hs);
    Vec4 v100 = V4( hs, -hs, -hs);
    Vec4 v110 = V4( hs,  hs, -hs);
    Vec4 v010 = V4(-hs,  hs, -hs);
    Vec4 v001 = V4(-hs, -hs,  hs);
    Vec4 v101 = V4( hs, -hs,  hs);
    Vec4 v111 = V4( hs,  hs,  hs);
    Vec4 v011 = V4(-hs,  hs,  hs);

    // -Z (front)  normal = (0,0,-1)
    addFace(/*a*/ v010, /*b*/ v110, /*c*/ v100, /*d*/ v000, N3(0, 0, -1));
    // +Z (back)   normal = (0,0,+1)
    addFace(/*a*/ v001, /*b*/ v101, /*c*/ v111, /*d*/ v011, N3(0, 0, +1));
    // -X (left)   normal = (-1,0,0)
    addFace(/*a*/ v000, /*b*/ v001, /*c*/ v011, /*d*/ v010, N3(-1, 0, 0));
    // +X (right)  normal = (+1,0,0)
    addFace(/*a*/ v100, /*b*/ v110, /*c*/ v111, /*d*/ v101, N3(+1, 0, 0));
    // -Y (bottom) normal = (0,-1,0)
    addFace(/*a*/ v000, /*b*/ v100, /*c*/ v101, /*d*/ v001, N3(0, -1, 0));
    // +Y (top)    normal = (0,+1,0)
    addFace(/*a*/ v010, /*b*/ v011, /*c*/ v111, /*d*/ v110, N3(0, +1, 0));

    // Mesh 구성 (정점 24, 인덱스 36)
    Mesh mesh;
    mesh.vertices = std::move(vertices);
    mesh.indices  = std::move(indices);

    // ▼ 주의: 이미 각 정점에 "면 노멀"을 박아놨으므로 별도 평균 노멀 계산은 불필요.
    // ComputeVertexNormals(mesh);  // 호출하지 마세요 (호출해도 면당 고유 정점이라 결과는 같긴 함)

    // 머티리얼
    auto mat = std::make_shared<Material>();
    mat->ambient_coef  = 1.0f;
    mat->diffuse_coef  = 1.0f;
    mat->specular_coef = 0.1f;
    mat->shininess     = 0.5f;
    mesh.material = mat;

    // 엔티티
    auto entity = std::make_shared<Entity>();
    entity->parts.push_back(std::move(mesh));
    entity->transform.position = Vec3(0, 0.0f, 0);
    entity->transform.scale    = Vec3(1.0f, 1.0f, 1.0f);
    entity->ComputeLocalAABB();
    return entity;
}

