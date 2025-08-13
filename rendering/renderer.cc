
#include "renderer.h"
#include "culling/clipper.h"
#include <algorithm>
#include <iostream>

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

void Renderer::SetLightingModel(std::unique_ptr<Lighting_Model> lighting)
{
    lighting_model = std::move(lighting);
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
    out.color = { v.color.r, v.color.g, v.color.b, v.color.a };

    return out;
}

inline float Renderer::GetTriangleSpace (const Projected_Vertex& A,
                                        const Projected_Vertex& B,
                                        const Projected_Vertex& C)
{
    return (B.x-A.x)*(C.y-A.y) - (B.y-A.y)*(C.x-A.x);
}

/*
    Barycentric Interpolation
    
    easy to implement

    if all weights are positive, current point is in triangle

    z interpolation is same with z from A,B,C plane equation (proved)
*/
void Renderer::RasterizeTriangle (const Vertex& v0, const Vertex& v1, const Vertex& v2)
{
    Projected_Vertex A = ProjectVertex(v0);
    Projected_Vertex B = ProjectVertex(v1);
    Projected_Vertex C = ProjectVertex(v2);
    Projected_Vertex P;

    int x_min = std::max(0, (int)std::floor(std::min({A.x, B.x, C.x})));
    int x_max = std::min(width-1, (int)std::ceil(std::max({A.x, B.x, C.x})));
    int y_min = std::max(0, (int)std::floor(std::min({A.y, B.y, C.y})));
    int y_max = std::min(height-1, (int)std::ceil(std::max({A.y, B.y, C.y})));

    float area = GetTriangleSpace(A, B, C);

    if (fabs(area) < 1e-6f) return;

    float inv_area = 1.0f/area;

    for (int y=y_min; y<=y_max; ++y)
    {
        for (int x=x_min; x<=x_max; ++x)
        {
            P.x = x+0.5f;
            P.y = y+0.5f;

            float w0 = GetTriangleSpace(A, B, P) * inv_area;
            float w1 = GetTriangleSpace(B, C, P) * inv_area;
            float w2 = GetTriangleSpace(C, A, P) * inv_area;

            if ( w0 < 0 || w1 < 0 || w2 < 0)
                continue;

            float z = 1.0f / (w0/A.z + w1/B.z + w2/C.z);

            int index = y*width + x;

            if ( z < z_buffer[index] )
            {
                z_buffer[index] = z;

                Color col;
                col.r = ((w0/A.z)*A.color.r + (w1/B.z)*B.color.r + (w2/C.z)*C.color.r) * z;
                col.g = ((w0/A.z)*A.color.g + (w1/B.z)*B.color.g + (w2/C.z)*C.color.g) * z;
                col.b = ((w0/A.z)*A.color.b + (w1/B.z)*B.color.b + (w2/C.z)*C.color.b) * z;
                col.a = ((w0/A.z)*A.color.a + (w1/B.z)*B.color.a + (w2/C.z)*C.color.a) * z;
                frame_buffer[index] = col;
            }
        }
    }
}

/*

조명·컬링·노멀 같은 “뷰 공간 로직”은 MV를, 
클립·래스터 같은 “투영 이후 로직”은 MVP를 씁니다. 
필요한 처리에 따라 둘 다 갖고 있는 게 가장 유연

M = MC to WC
V = WC to VC
P = VC to projection 
*/

void Renderer::DrawMesh (const std::vector<std::shared_ptr<Light>>& lights, 
                         const Vec3& camera_pos,
                         const Vec3& ambient,
                         const Mesh& mesh,
                         const Clipper& clipper, 
                         Mat4x4 M, 
                         Mat4x4 V, 
                         Mat4x4 P)
{
    Mat4x4 PV = P * V;
    Mat3x3 InverseTranspose_M = M.TopLeft3x3().InverseTranspose(); 
    Mesh out_mesh = mesh;

    /*
        MC to WC
    */
    for (size_t i=0; i<out_mesh.vertices.size(); ++i)
    {
        out_mesh.vertices[i].position = M * out_mesh.vertices[i].position;
        out_mesh.vertices[i].normal = Vec3::Normalize( InverseTranspose_M * out_mesh.vertices[i].normal );
    }

    //DebugCheckNormals(out_mesh);

    /*
        Illumination
    */
    for (size_t i=0; i<out_mesh.vertices.size(); ++i)
    {
        out_mesh.vertices[i].color = lighting_model->Shade(
            out_mesh.vertices[i].position.ToVec3(),
            out_mesh.vertices[i].normal,
            camera_pos - out_mesh.vertices[i].position.ToVec3(),
            *mesh.material,
            lights,
            out_mesh.vertices[i].color,
            ambient
        );
    }

    /*
    for (auto& v : out_mesh.vertices)
    {
        printf("out r=%.2f g=%.2f b=%.2f\n", v.color.r, v.color.g, v.color.b);
    }
        */

    /*
        Clipping
    */

    Mesh transformed_mesh = clipper.ClipMesh(out_mesh);
    /*
    for (auto& v : transformed_mesh.vertices)
    {
        printf("transformed r=%.2f g=%.2f b=%.2f\n", v.color.r, v.color.g, v.color.b);
    }
        */

    /*
        Projection
    */
    for (auto& v : transformed_mesh.vertices)
        v.position = PV * v.position;
 
    for (size_t i=0; i+2<transformed_mesh.indices.size(); i+=3) 
    {
        RasterizeTriangle(transformed_mesh.vertices[transformed_mesh.indices[i+0]], 
                          transformed_mesh.vertices[transformed_mesh.indices[i+1]], 
                          transformed_mesh.vertices[transformed_mesh.indices[i+2]]);
    }
}

void Renderer::Render(const Scene& scene)
{
    ClearBuffers();

    std::vector<std::shared_ptr<Light>> lights = scene.GetLightManager()->GetLights();

    Vec3 ambient = scene.GetLightManager()->GetAmbient();

    std::shared_ptr<Camera> camera = scene.GetCamera();

    Mat4x4 V = camera->GetViewMatrix();

    Mat4x4 P = camera->GetProjMatrix();

    Vec3 camera_pos = camera->GetPosition();

    Clipper clipper;
    clipper.ExtractFrustumPlanes(P*V);

    /*
    Vec3 cam = camera->GetPosition();
    printf("camera pos: (%.2f, %.2f, %.2f)\n",
        cam.x, cam.y, cam.z);

    auto planes = clipper.GetFrustumPlanes();
    for (int i = 0; i < 6; ++i) {
        Vec4 &p = planes[i];
        printf("Plane %d: normal=(%.2f,%.2f,%.2f), d=%.2f\n",
            i, p.x, p.y, p.z, p.w);
    }
    */

    // AABB Culling
    for ( auto& e : scene.GetEntities() )
    {
        AABB world_aabb = e->GetLocalAABB().MatrixConversion(e->GetLocalToWorldMatrix());
        
        /*
        auto local = e->GetLocalAABB();
        auto world = world_aabb;
        printf("Local AABB: min(%.2f,%.2f,%.2f) max(%.2f,%.2f,%.2f)\n",
            local.min.x, local.min.y, local.min.z,
            local.max.x, local.max.y, local.max.z);
        printf("World AABB: min(%.2f,%.2f,%.2f) max(%.2f,%.2f,%.2f)\n",
            world.min.x, world.min.y, world.min.z,
            world.max.x, world.max.y, world.max.z);

        printf("before aabb\n");
        */
        if ( clipper.IsAABBVisible(world_aabb) == false )
            continue;

        //printf("after aabb\n");

        for ( auto& mesh : e->parts )
        {
            DrawMesh(lights, camera_pos, ambient, mesh, clipper, e->GetLocalToWorldMatrix(), V, P);
        }
    }
}

/*

    void Renderer::DrawMesh (const Mesh& mesh, const Clipper& clipper, Mat4x4 M, Mat4x4 V, Mat4x4 P)
{
    Mat4x4 PV = P * V;
    Mat3x3 InverseTranspose_M = M.TopLeft3x3().InverseTranspose(); 
    Mesh out_mesh = mesh;
    Vec3 camera_pos = scene->GetCamera()->GetPosition();


    for (size_t i=0; i<out_mesh.vertices.size(); ++i)
    {
        out_mesh.vertices[i].position = M * out_mesh.vertices[i].position;
        out_mesh.vertices[i].normal = InverseTranspose_M * out_mesh.vertices[i].normal;

        float intensity = lighting_model->Shade(
            out_mesh.vertices[i].position.ToVec3(),
            out_mesh.vertices[i].normal,
            camera_pos - out_mesh.vertices[i].position.ToVec3(),
            *mesh.material,
            scene->GetLightManager()->GetLights()
        );

        out_mesh.vertices[i].color.r *= intensity;
        out_mesh.vertices[i].color.g *= intensity;
        out_mesh.vertices[i].color.b *= intensity;
    }



    Mesh transformed_mesh = clipper.ClipMesh(out_mesh);


    for (auto& v : out_mesh.vertices)
        v.position = PV * v.position;

    for (size_t i=0; i+2<mesh.indices.size(); i+=3) 
    {
        RasterizeTriangle(transformed_mesh.vertices[transformed_mesh.indices[i+0]], 
                          transformed_mesh.vertices[transformed_mesh.indices[i+1]], 
                          transformed_mesh.vertices[transformed_mesh.indices[i+2]]);
    }
}

void Renderer::Render(const Scene& scene)
{
    std::vector<std::shared_ptr<Entity>> visible;
    visible.reserve(scene.GetEntities().size());

    std::shared_ptr<Camera> camera = scene.GetCamera();

    Mat4x4 V = camera->GetViewMatrix();
    Mat4x4 P = camera->GetProjMatrix();

    Frustum frustum;
    frustum.ExtractFrustumPlanes(V*P);

    Clipper clipper;
    clipper.SetFrustumPlanes(frustum.GetFrustumPlanes());

    // AABB Culling
    for ( auto& e : scene.GetEntities() )
    {
        AABB world_aabb = e->GetLocalAABB().MatrixConversion(e->GetLocalToWorldMatrix());

        if ( clipper.IsAABBVisible(world_aabb) == true )
            visible.push_back(e);
    }

    for ( auto& e : visible )
    {
        // Draw Mesh
        for ( auto& mesh : e->parts )
            DrawMesh(mesh, clipper, e->GetLocalToWorldMatrix(), V, P);
    }
}

*/