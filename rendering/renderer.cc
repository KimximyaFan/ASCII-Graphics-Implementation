
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
    
    //    Clip Position -> Nomalized Device Coordinate
    
    float invW = 1.0f / v.position.w;
    float ndcX = v.position.x * invW;
    float ndcY = v.position.y * invW;
    float ndcZ = v.position.z * invW;

    
    //    overhead
    
    //Mat4x4 viewport_matrix = Mat4x4::ViewportTransformation(0, width, 0, height);
    
    
    //    NDC -> Screen Coordinate
    
    Projected_Vertex out;
    out.invW = invW;
    out.x = (ndcX*0.5f + 0.5f) * width;
    // 배열의 y축은 위에서 아래로 가기때문에, y를 뒤집어줘야함, height - y 
    out.y = (1.0f - (ndcY*0.5f + 0.5f)) * height;
    out.z = ndcZ*0.5f + 0.5f;

    out.r_over_w = v.color.r * invW;
    out.g_over_w = v.color.g * invW;
    out.b_over_w = v.color.b * invW;
    out.u_over_w = v.uv.x * invW;
    out.v_over_w = v.uv.y * invW;

    return out;
}

/*
Projected_Vertex Renderer::ProjectVertex (const Vertex& v)
{
    // v는 이미 PV 곱해진 "clip 좌표"가 들어온다고 가정
    float invW = 1.0f / v.position.w;
    float ndcX = v.position.x * invW;
    float ndcY = v.position.y * invW;
    float ndcZ = v.position.z * invW;

    Projected_Vertex out{};
    out.invW  = invW;
    out.x     = (ndcX*0.5f + 0.5f) * width;
    // 화면 y는 위->아래 증가: NDC y를 뒤집어서 스크린으로
    out.y     = (1.0f - (ndcY*0.5f + 0.5f)) * height;
    out.z_ndc = ndcZ;
    out.z     = ndcZ*0.5f + 0.5f; // [0,1]

    // 퍼스펙티브 보정용 속성
    out.r_over_w = v.color.r * invW;
    out.g_over_w = v.color.g * invW;
    out.b_over_w = v.color.b * invW;
    out.u_over_w = v.uv.x     * invW;
    out.v_over_w = v.uv.y     * invW;

    // (디버깅용으로만 쓰는 값이면 유지, 아니면 불필요)
    out.u = v.uv.x;
    out.v = v.uv.y;
    return out;
}
*/
inline float Renderer::GetTriangleSpace (const Projected_Vertex& A,
                                        const Projected_Vertex& B,
                                        const Projected_Vertex& C)
{
    return (B.y-A.y)*(C.x-A.x) - (B.x-A.x)*(C.y-A.y);
}


static inline float EdgeFn(float ax, float ay, float bx, float by, float px, float py)
{
    // (b - a) ⟂ · (p - a)  (스크린 좌표계 기준)
    return (by - ay)*(px - ax) - (bx - ax)*(py - ay);
}
static inline bool IsTopLeft(float ax, float ay, float bx, float by)
{
    // 수평 에지는 왼쪽 끝을 포함, 수직/기울어진 에지는 위쪽 에지 포함
    // 우리의 스크린좌표는 y가 아래로 증가하므로,
    //  - "상단(top)" 에지는 ay < by 인 에지
    //  - "좌측(left)" 에지는 ay == by && ax < bx 인 에지
    if (ay == by) return ax < bx; // 수평 에지: 왼쪽 끝 포함
    return ay < by;               // 상단 에지 포함
}


void Renderer::RasterizeTriangle2 (const Vertex& v0, const Vertex& v1, const Vertex& v2, const Texture* texture)
{
    Projected_Vertex A = ProjectVertex(v0);
    Projected_Vertex B = ProjectVertex(v1);
    Projected_Vertex C = ProjectVertex(v2);

    // 삼각형 외접 bbox
    int x_min = std::max(0, (int)std::floor(std::min({A.x, B.x, C.x})));
    int x_max = std::min(width-1,  (int)std::ceil (std::max({A.x, B.x, C.x})));
    int y_min = std::max(0, (int)std::floor(std::min({A.y, B.y, C.y})));
    int y_max = std::min(height-1, (int)std::ceil (std::max({A.y, B.y, C.y})));

    // 전체 삼각형 부호있는 면적
    float area = EdgeFn(A.x, A.y, B.x, B.y, C.x, C.y);
    if (std::fabs(area) < 1e-8f) return;

    // Top-Left 규칙에서 에지 포함 판정에 사용할 보정:
    // 면적 부호를 양으로 맞추고(내부 계산 단순화), 에지 함수도 동일 부호로 맞춤
    float orient = (area > 0.0f) ? 1.0f : -1.0f;
    area *= orient;

    // 에지의 top/left 여부 미리 계산
    bool eAB_TL = IsTopLeft(A.x, A.y, B.x, B.y);
    bool eBC_TL = IsTopLeft(B.x, B.y, C.x, C.y);
    bool eCA_TL = IsTopLeft(C.x, C.y, A.x, A.y);

    constexpr float Z_EPS = 1e-7f;

    for (int y = y_min; y <= y_max; ++y)
    {
        float py = y + 0.5f; // 픽셀 센터
        for (int x = x_min; x <= x_max; ++x)
        {
            float px = x + 0.5f;

            // 에지 함수(부호 정규화)
            float eAB = orient * EdgeFn(A.x, A.y, B.x, B.y, px, py);
            float eBC = orient * EdgeFn(B.x, B.y, C.x, C.y, px, py);
            float eCA = orient * EdgeFn(C.x, C.y, A.x, A.y, px, py);

            // Top-Left 규칙: >0 포함, ==0 이면 top/left 에지만 포함
            if ( (eAB < 0 || (eAB == 0 && !eAB_TL)) ||
                 (eBC < 0 || (eBC == 0 && !eBC_TL)) ||
                 (eCA < 0 || (eCA == 0 && !eCA_TL)) )
                continue;

            // barycentric (정규화)
            float w0 = eBC / area; // A에 대응 (PBC)
            float w1 = eCA / area; // B에 대응 (PCA)
            float w2 = eAB / area; // C에 대응 (PAB)
            // (합은 1이지만 부동오차로 약간 어긋날 수 있음)

            // 깊이: 스크린에서 선형
            float z = w0*A.z + w1*B.z + w2*C.z;

            int index = y*width + x;
            if (z > z_buffer[index] - Z_EPS) continue; // 타이브레이커

            // 퍼스펙티브 보정
            float denom = w0*A.invW + w1*B.invW + w2*C.invW;
            if (!std::isfinite(denom)) continue;
            if (std::fabs(denom) < 1e-20f) continue; // 안전

            float u = (w0*A.u_over_w + w1*B.u_over_w + w2*C.u_over_w ) / denom; // ← 아래서 수정
            float v = (w0*A.v_over_w + w1*B.v_over_w + w2*C.v_over_w) / denom;

            // ↑ 오타 방지: C 쪽 u_over_w와 v_over_w를 정확히 사용하세요.
            // 올바른 라인:
            // float u = (w0*A.u_over_w + w1*B.u_over_w + w2*C.u_over_w) / denom;
            // float v = (w0*A.v_over_w + w1*B.v_over_w + w2*C.v_over_w) / denom;

            // 텍스처 샘플
            Color tex = texture ? texture->Sample(u, v) : Color{1,1,1,1};

            // (선택) 정점색도 퍼스펙티브 보정
            Color col;
            col.r = (w0*A.r_over_w + w1*B.r_over_w + w2*C.r_over_w) / denom;
            col.g = (w0*A.g_over_w + w1*B.g_over_w + w2*C.g_over_w) / denom;
            col.b = (w0*A.b_over_w + w1*B.b_over_w + w2*C.b_over_w) / denom;
            col.a = 1.0f;

            z_buffer[index] = z;
            frame_buffer[index] = col * tex;
        }
    }
}


/*
    Barycentric Interpolation
    
    easy to implement

    if all weights are positive, current point is in triangle

    z interpolation is same with z from A,B,C plane equation (proved)
*/

void Renderer::RasterizeTriangle (const Vertex& v0, const Vertex& v1, const Vertex& v2, const Texture* texture)
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

    float orient = area > 0.0f ? 1.0f : -1.0f;
    area *= orient;

    float inv_area = 1.0f/area;

    for (int y=y_min; y<=y_max; ++y)
    {
        P.y = y+0.5f;

        for (int x=x_min; x<=x_max; ++x)
        {
            P.x = x+0.5f;

            float w0 = orient * GetTriangleSpace(B, C, P) * inv_area;
            float w1 = orient * GetTriangleSpace(C, A, P) * inv_area;
            float w2 = orient * GetTriangleSpace(A, B, P) * inv_area;

            if ( w0 < 0.0f || w1 < 0.0f || w2 < 0.0f)
                continue;

            //float denom = w0*A.invW + w1*B.invW + w2*C.invW;

            //if (denom <= 0.0f) continue;

            float z = w0*A.z + w1*B.z + w2*C.z;
            
            //float z_ndc = w0*A.z_ndc + w1*B.z_ndc + w2*C.z_ndc;   // [-1,1]
            //float z     = z_ndc * 0.5f + 0.5f;

            //float z = 1.0f / (w0/A.z + w1/B.z + w2/C.z);

            int index = y*width + x;
            
            /*
            Color col;

            col.r = ((w0/A.z)*A.color.r + (w1/B.z)*B.color.r + (w2/C.z)*C.color.r) * z;
            col.g = ((w0/A.z)*A.color.g + (w1/B.z)*B.color.g + (w2/C.z)*C.color.g) * z;
            col.b = ((w0/A.z)*A.color.b + (w1/B.z)*B.color.b + (w2/C.z)*C.color.b) * z;
            col.a = ((w0/A.z)*A.color.a + (w1/B.z)*B.color.a + (w2/C.z)*C.color.a) * z;

            float buffer_intensity = frame_buffer[index].r + frame_buffer[index].g + frame_buffer[index].b;
            float current_intensity = col.r + col.g + col.b;

            if ( buffer_intensity < current_intensity )
                frame_buffer[index] = col;
            */
            
            if ( z >= z_buffer[index] ) continue;

            float denom = w0*A.invW + w1*B.invW + w2*C.invW;

            //if (!(denom > 0.0f) || !std::isfinite(denom)) continue;
            if (!std::isfinite(denom) || std::fabs(denom) < 1e-12f) continue;

            //col.r = ((w0/A.z)*A.color.r + (w1/B.z)*B.color.r + (w2/C.z)*C.color.r) * z;
            //col.g = ((w0/A.z)*A.color.g + (w1/B.z)*B.color.g + (w2/C.z)*C.color.g) * z;
            //col.b = ((w0/A.z)*A.color.b + (w1/B.z)*B.color.b + (w2/C.z)*C.color.b) * z;
            //col.a = ((w0/A.z)*A.color.a + (w1/B.z)*B.color.a + (w2/C.z)*C.color.a) * z;
            
            float u = (w0*A.u_over_w + w1*B.u_over_w + w2*C.u_over_w) / denom;
            float v = (w0*A.v_over_w + w1*B.v_over_w + w2*C.v_over_w) / denom;

            //float u = w0*A.u + w1*B.u + w2*C.u;
            //float v = w0*A.v + w1*B.v + w2*C.v;

            Color tex = texture ? texture->Sample(u, v) : Color{1,1,1,1};
            
            Color col;
            col.r = (w0*A.r_over_w + w1*B.r_over_w + w2*C.r_over_w) / denom;
            col.g = (w0*A.g_over_w + w1*B.g_over_w + w2*C.g_over_w) / denom;
            col.b = (w0*A.b_over_w + w1*B.b_over_w + w2*C.b_over_w) / denom;
            col.a = 1.0f;

            Color out = col * tex;
            //col.a = (w0*A.color.a*A.invW + w1*B.color.a*B.invW + w2*C.color.a*C.invW) / denom;
            
            z_buffer[index] = z;
            frame_buffer[index] = out;
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
                         const Texture* texture, 
                         Mat4x4 M, 
                         Mat4x4 V, 
                         Mat4x4 P
                        )
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

    out_mesh = clipper.BackFaceCull_View(out_mesh, V);
    //out_mesh = clipper.BackFaceCull_View(out_mesh, camera_pos);
    //DebugCheckNormals(out_mesh);
    //out_mesh = clipper.BackFaceRemoval(out_mesh, view_direction);
    //out_mesh = clipper.BackFaceRemoval2(out_mesh, V);

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
                          transformed_mesh.vertices[transformed_mesh.indices[i+2]],
                          texture);
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

    Vec3 view_direction = camera->GetViewDirection();

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
            DrawMesh(lights, camera_pos, ambient, mesh, clipper, scene.GetTextureManager()->GetTexture(mesh.material->texture_handle), e->GetLocalToWorldMatrix(), V, P);
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

/*
static inline Vec3 ExpectedFaceNormalAfterY(float deg, const Vec3& N0 MC face normal
{
    const float rad = deg * 3.14159265f / 180.0f;
    const float c = std::cos(rad), s = std::sin(rad);
    // Y축 회전 행렬 R_y(θ) 적용 (X,Z만 변함)
    Vec3 N;
    N.x =  c * N0.x + 0.0f * N0.y + s * N0.z;
    N.y =  0.0f;
    N.y +=      N0.y; // Y는 보존
    N.z = -s * N0.x + 0.0f * N0.y + c * N0.z;
    return Vec3::Normalize(N);
}

void Renderer::DebugShadeNoRaster_DirectionalOnly(const Scene& scene, float angle_deg, const Vec3& dir_param)
{
    const auto lights  = scene.GetLightManager()->GetLights();
    const Vec3 ambient = scene.GetLightManager()->GetAmbient();
    const auto cam     = scene.GetCamera();
    const Vec3 cam_pos = cam->GetPosition();

    // 디렉셔널 기대치용 고정 L (네 구현과 동일한 규칙)
    const Vec3 L_dir = Vec3::Normalize(dir_param);

    for (auto& e : scene.GetEntities())
    {
        const Mat4x4 M   = e->GetLocalToWorldMatrix();
        const Mat3x3 NIT = M.TopLeft3x3().InverseTranspose();

        for (const auto& mesh : e->parts)
        {
            for (size_t i = 0; i < mesh.vertices.size(); ++i)
            {
                const Vertex& vin = mesh.vertices[i];

                // --- 실제 파이프라인 값 ---
                const Vec3 P = (M * vin.position).ToVec3();
                const Vec3 N = Vec3::Normalize(NIT * vin.normal);
                const Vec3 V = Vec3::Normalize(cam_pos - P);

                float sum_diff = 0.0f, sum_spec = 0.0f, NL_last = 0.0f, NH_last = 0.0f;
                for (auto& L : lights)
                {
                    const float I  = L->GetIntensity(P);
                    const Vec3  Ld = Vec3::Normalize(L->GetDirection(P)); // == L_dir일 것
                    const float NL = std::max(0.0f, Vec3::Dot(N, Ld));
                    if (NL > 0.0f)
                    {
                        sum_diff += mesh.material->diffuse_coef * I * NL;
                        if (mesh.material->shininess > 0.0f && mesh.material->specular_coef > 0.0f)
                        {
                            const Vec3 H  = Vec3::Normalize(Ld + V);
                            const float NH = std::max(0.0f, Vec3::Dot(N, H));
                            sum_spec += mesh.material->specular_coef * I * std::pow(NH, mesh.material->shininess);
                            NH_last = NH;
                        }
                        NL_last = NL;
                    }
                }
                Color actual;
                actual.r = vin.color.r * (sum_diff + mesh.material->ambient_coef * ambient.x) + sum_spec;
                actual.g = vin.color.g * (sum_diff + mesh.material->ambient_coef * ambient.y) + sum_spec;
                actual.b = vin.color.b * (sum_diff + mesh.material->ambient_coef * ambient.z) + sum_spec;

                // --- 해석적 기대치 (Y-회전만 가정) ---
                // 큐브는 면당 고정 노멀(±X,±Y,±Z)라서 vin.normal이 바로 N0
                Vec3 N0 = vin.normal; // MC face normal
                Vec3 Nexp = ExpectedFaceNormalAfterY(angle_deg, N0); // 회전만 반영
                const float NL_exp = std::max(0.0f, Vec3::Dot(Nexp, L_dir));
                float diff_exp = mesh.material->diffuse_coef * 1.0f * NL_exp;
                float spec_exp = 0.0f;
                if (NL_exp > 0.0f && mesh.material->shininess > 0.0f && mesh.material->specular_coef > 0.0f)
                {
                    const Vec3 H = Vec3::Normalize(L_dir + V);
                    const float NH = std::max(0.0f, Vec3::Dot(Nexp, H));
                    spec_exp = mesh.material->specular_coef * 1.0f * std::pow(NH, mesh.material->shininess);
                }
                Color expect;
                expect.r = vin.color.r * (diff_exp + mesh.material->ambient_coef * ambient.x) + spec_exp;
                expect.g = vin.color.g * (diff_exp + mesh.material->ambient_coef * ambient.y) + spec_exp;
                expect.b = vin.color.b * (diff_exp + mesh.material->ambient_coef * ambient.z) + spec_exp;

                // --- 차이 프린트 ---
                auto pr = [&](const char* tag, const Color& C){
                    printf("%s C(%.3f, %.3f, %.3f)", tag, C.r, C.g, C.b);
                };
                printf("[deg=%3.0f] v%02zu  P(%.2f,%.2f,%.2f)  N_act(%.3f,%.3f,%.3f)  N_exp(%.3f,%.3f,%.3f)  |NL_act=%.3f|\n",
                       angle_deg, i, P.x, P.y, P.z, N.x, N.y, N.z, Nexp.x, Nexp.y, Nexp.z, NL_last);
                pr("  actual ", actual);
                printf("   "); pr("expect ", expect);
                printf("   Δ(%.3f, %.3f, %.3f)\n", actual.r - expect.r, actual.g - expect.g, actual.b - expect.b);
            }
        }
    }
}
*/