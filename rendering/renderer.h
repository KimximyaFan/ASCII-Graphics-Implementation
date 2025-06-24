#pragma once

#include <vector>
#include "primitive.h"
#include "matrix.h"
#include "color.h"
#include "projected_vertex.h"

class Renderer
{
public:
    static const Color clear_color;
    static const float clear_depth;

    Renderer (int width, int height);

    void ClearBuffers ();

    // mvp : Model View Projection
    void DrawMesh (const Mesh& mesh, Mat4x4 mvp);

    const std::vector<Color>& GetFrameBuffer () const;

    const std::vector<float>& GetZBuffer () const;

private:
    int width, height;
    Mat4x4 viewport_matrix;
    std::vector<Color> frame_buffer;
    std::vector<float> z_buffer;

    Projected_Vertex ProjectVertex (const Vertex& v);

    inline float GetTriangleSpace (const Projected_Vertex& A, const Projected_Vertex& B, const Projected_Vertex& C);

    void RasterizeTriangle (const Vertex& v0, const Vertex& v1, const Vertex& v2);
};
