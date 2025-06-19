#pragma once

#include <vector>
#include "primitive.h"
#include "matrix.h"
#include "color.h"



class Renderer
{
public:
    Renderer (int width, int height);

    void ClearBuffers ();

    // mvp : Model View Projection
    void DrawMesh (const Mesh& mesh, Mat4x4 mvp);

    const std::vector<Color>& const GetFrameBuffer ();

    const std::vector<float>& const GetZBuffer ();

private:
    int width, height;
    std::vector<Color> frame_buffer;
    std::vector<float> z_buffer;

    struct Projected_Vertex
    {
        float x, y, z;
        Color c;
    };

    void RasterizeTriangle (const Vertex& v0, const Vertex& v1, const Vertex& v2);

    Projected_Vertex ProjectVertex (const Vertex& v);
};
