#pragma once

#include <vector>
#include <memory> 
#include "primitive.h"
#include "matrix.h"
#include "color.h"
#include "projected_vertex.h"
#include "light models/lighting_model.h"
#include "scene/scene.h"
#include "culling/clipper.h"

class Renderer
{
public:
    static const Color clear_color;
    static const float clear_depth;

    Renderer (int width, int height);

    void ClearBuffers ();

    void SetLightingModel (std::unique_ptr<Lighting_Model> lighting);

    // mvp : Model View Projection
    void DrawMesh (const Mesh& mesh, const Clipper& clipper, Mat4x4 M, Mat4x4 V, Mat4x4 P);

    const std::vector<Color>& GetFrameBuffer () const;

    const std::vector<float>& GetZBuffer () const;

    void Render(const Scene& scene);

private:
    int width, height;
    Mat4x4 viewport_matrix;
    std::vector<Color> frame_buffer;
    std::vector<float> z_buffer;
    std::unique_ptr<Lighting_Model> lighting_model;
    std::unique_ptr<Scene> scene;

    Projected_Vertex ProjectVertex (const Vertex& v);

    inline float GetTriangleSpace (const Projected_Vertex& A, const Projected_Vertex& B, const Projected_Vertex& C);

    void RasterizeTriangle (const Vertex& v0, const Vertex& v1, const Vertex& v2);
};
