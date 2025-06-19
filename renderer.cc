#pragma once

#include "renderer.h"

Renderer::Renderer (int width, int height) {}

void Renderer::ClearBuffers ()
{

}

void Renderer::DrawMesh (const Mesh& mesh, Mat4x4 mvp)
{

}

const std::vector<Color>& Renderer::GetFrameBuffer () const
{
    return frame_buffer;
}

const std::vector<float>& Renderer::GetZBuffer () const
{
    return z_buffer;
}

void RasterizeTriangle (const Vertex& v0, const Vertex& v1, const Vertex& v2)
{

}

Renderer::Projected_Vertex Renderer::ProjectVertex (const Vertex& v)
{
    Projected_Vertex pv;
    return pv;
}