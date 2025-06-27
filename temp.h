#pragma once

#include "renderer.h"
#include "matrix.h"
#include "clipper.h"
#include <vector>

void Renderer::DrawMesh(
    const Mesh& mesh,
    const Mat4x4& modelMat,
    const Mat4x4& viewMat,
    const Mat4x4& projectionMat
) {
    // 1) Compute ModelView and MVP
    Mat4x4 modelView = viewMat * modelMat;
    Mat4x4 mvp       = projectionMat * modelView;

    // 2) Per-vertex transform and illumination
    Mesh projMesh;
    projMesh.indices = mesh.indices;
    projMesh.vertices.resize(mesh.vertices.size());

    // Precompute normal matrix (inverse-transpose of modelView)
    Mat4x4 normalMat = modelView.inverse().transpose();

    for (size_t i = 0; i < mesh.vertices.size(); ++i) {
        const Vertex& inV = mesh.vertices[i];
        Vertex outV = inV;

        // View-space position
        Vec4 viewPos4     = modelView * inV.position;
        Vec3 viewPos      = viewPos4.xyz();
        // Clip-space position for clipping and rasterization
        outV.position     = mvp * inV.position;

        // View-space normal
        Vec3 viewNormal   = Vec3::Normalize((normalMat * Vec4(inV.normal, 0.0f)).xyz());

        // Per-vertex illumination (camera at origin in view space)
        float intensity = lighting_model->Shade(
            viewPos,
            viewNormal,
            Vec3::Normalize(-viewPos),
            material,
            scene_lights
        );
        // Store intensity in alpha channel
        outV.color.w     = intensity;

        projMesh.vertices[i] = outV;
    }

    // 3) Clip in clip space (before perspective divide)
    Clipper clipper(projectionMat);
    Mesh clipped = clipper.ClipMesh(projMesh);

    // 4) Rasterize triangles from clipped mesh
    for (size_t i = 0; i + 2 < clipped.indices.size(); i += 3) {
        const Vertex& v0 = clipped.vertices[clipped.indices[i    ]];
        const Vertex& v1 = clipped.vertices[clipped.indices[i + 1]];
        const Vertex& v2 = clipped.vertices[clipped.indices[i + 2]];
        RasterizeTriangle(v0, v1, v2);
    }
}
