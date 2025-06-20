#pragma once

#include "renderer.h"
#include "matrix.h"

const Color Renderer::clear_color = {0, 0, 0, 0};
const float Renderer::clear_depth = 1.0f;

Renderer::Renderer (int w, int h)
    : width(w), height(h),
    frame_buffer(w*h),
    z_buffer(w*h)
    { viewport_matrix = Mat4x4::ViewportTransformation(0, w, 0, h); }

const std::vector<Color>& Renderer::GetFrameBuffer () const
{
    return frame_buffer;
}

const std::vector<float>& Renderer::GetZBuffer () const
{
    return z_buffer;
}

void Renderer::ClearBuffers ()
{
    std::fill(frame_buffer.begin(), frame_buffer.end(), clear_color);
    std::fill(z_buffer.begin(), z_buffer.end(), clear_depth);
}

Projected_Vertex Renderer::ProjectVertex (const Vertex& v)
{
    /*
        Clip Position -> Nomalized Device Coordinate
    */
    float invW = 1.0f / v.position.w;
    float ndcX = v.position.x * invW;
    float ndcY = v.position.y * invW;
    float ndcZ = v.position.z * invW;

    /*
        overhead
    */
    //Mat4x4 viewport_matrix = Mat4x4::ViewportTransformation(0, width, 0, height);
    
    /*
        NDC -> Screen Coordinate
    */

    Projected_Vertex out;

    out.x = (ndcX*0.5f + 0.5f) * width;
    // 배열의 y축은 위에서 아래로 가기때문에, y를 뒤집어줘야함, height - y 
    out.y = (1.0f - (ndcY*0.5f + 0.5f)) * height;
    out.z = ndcZ*0.5f + 0.5f;
    out.c = { v.color.x, v.color.y, v.color.z, v.color.w };

    return out;
}

void Renderer::RasterizeTriangle (const Vertex& v0, const Vertex& v1, const Vertex& v2)
{
    Projected_Vertex pv0 = ProjectVertex(v0);
    Projected_Vertex pv1 = ProjectVertex(v1);
    Projected_Vertex pv2 = ProjectVertex(v2);

    
}

void Renderer::DrawMesh (const Mesh& mesh, Mat4x4 mvp)
{
    // deep copy
    std::vector<Vertex> transformed_vertices = mesh.vertices;

    for (auto& v : transformed_vertices)
        v.position = mvp * v.position;

    for (size_t i=0; i+2<mesh.indices.size(); i+=3) 
    {
        RasterizeTriangle(transformed_vertices[mesh.indices[i+0]], 
                          transformed_vertices[mesh.indices[i+1]], 
                          transformed_vertices[mesh.indices[i+2]]);
    }
}