#pragma once

#include <memory>
#include "scene/entity.h"
#include "geometry/vertex.h"
#include "material/material.h"
#include "math/vector.h"
#include "material/texture.h"
#include <iostream>
#include <vector>

std::shared_ptr<Entity> CreateCubeEntity_Flat24(float size = 1.0f)
{
    float hs = size * 0.5f; // half size
    const Color faceColor = {0.5f, 0.5f, 0.5f, 0.0f}; // per-vertex color (optional tint)
    const float uv_scale_u = 1.0f; // 필요 시 타일링 배수 조절
    const float uv_scale_v = 1.0f;

    auto V4 = [&](float x, float y, float z) { return Vec4{x, y, z, 1.0f}; };
    auto N3 = [&](float x, float y, float z) { return Vec3{x, y, z}; };

    // 위치→UV 변환 (면 법선에 따라 투영 평면 선택)
    auto UVof = [&](const Vec4& p, const Vec3& n) -> Vec2 {
        float u = 0.0f, v = 0.0f;
        if (n.z != 0.0f) {            // ±Z 면 → (X,Y)
            u = (p.x / hs + 1.0f) * 0.5f;
            v = (p.y / hs + 1.0f) * 0.5f;
        } else if (n.x != 0.0f) {     // ±X 면 → (Z,Y)
            u = (p.z / hs + 1.0f) * 0.5f;
            v = (p.y / hs + 1.0f) * 0.5f;
        } else {                      // ±Y 면 → (X,Z)
            u = (p.x / hs + 1.0f) * 0.5f;
            v = (p.z / hs + 1.0f) * 0.5f;
        }
        return Vec2{ u * uv_scale_u, v * uv_scale_v };
    };

    std::vector<Vertex>   vertices; vertices.reserve(24);
    std::vector<uint32_t> indices;  indices.reserve(36);

    // a,b,c,d: 바깥에서 볼 때 CCW, n: 면 법선
    auto addFace = [&](const Vec4& a, const Vec4& b, const Vec4& c, const Vec4& d, const Vec3& n) {
        uint32_t base = static_cast<uint32_t>(vertices.size());
        vertices.push_back(Vertex{ a, n, UVof(a,n), faceColor }); // 0
        vertices.push_back(Vertex{ b, n, UVof(b,n), faceColor }); // 1
        vertices.push_back(Vertex{ c, n, UVof(c,n), faceColor }); // 2
        vertices.push_back(Vertex{ d, n, UVof(d,n), faceColor }); // 3
        // 두 개의 삼각형
        indices.push_back(base + 0);
        indices.push_back(base + 1);
        indices.push_back(base + 2);
        indices.push_back(base + 0);
        indices.push_back(base + 2);
        indices.push_back(base + 3);
    };

    // 8 모서리 좌표
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

    // Mesh 구성
    Mesh mesh;
    mesh.vertices = std::move(vertices);
    mesh.indices  = std::move(indices);

    // 머티리얼 (BRICK 텍스처 사용)
    auto mat = std::make_shared<Material>();
    mat->ambient_coef  = 1.0f;
    mat->diffuse_coef  = 1.0f;
    mat->specular_coef = 0.1f;
    mat->shininess     = 0.5f;
    mat->texture_handle = Texture_Handle::BRICK; // ← 여기!
    mesh.material = mat;

    // 엔티티
    auto entity = std::make_shared<Entity>();
    entity->parts.push_back(std::move(mesh));
    entity->transform.position = Vec3(0, 0.0f, 0);
    entity->transform.scale    = Vec3(1.0f, 1.0f, 1.0f);
    entity->ComputeLocalAABB();
    return entity;
}


