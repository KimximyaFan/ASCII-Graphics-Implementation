
#include "clipper.h"
#include <iostream>

const std::array<Vec4, 6>& Clipper::GetFrustumPlanes()
{
    return frustum_planes;
}

bool Clipper::IsVertexInside(const Vertex& vertex, const Vec4& plane)
{
    return Vec4::Dot(vertex.position, plane) >= 0.0f;
}

/*
    P1, P2 exist
    line equeation can be expressed by this
    P = P1 + (P2 - P1) * u

    P is the Vertex on line made by P1 and P2
    we can find Intersect Vertex by put P on plane equation

    we can find u by this equation

    dir_vec = P2 - P1
    
    plane·( P1 + u*dir_vec ) = 0

    u = - plane·P1 / plane·dir_vec
*/
Vertex Clipper::GetIntersectVertex(const Vec4& plane, const Vertex& a, const Vertex& b)
{
    Vec4 direction_vector = Vec4(
        b.position.x - a.position.x,
        b.position.y - a.position.y,
        b.position.z - a.position.z,
        b.position.w - a.position.w
    );

    float u = -Vec4::Dot(a.position, plane) / Vec4::Dot(plane, direction_vector);

    Vertex result; 
    result.position = a.position + direction_vector * u;
    result.color = a.color + (b.color - a.color) * u;
    result.normal = Vec3::Normalize(a.normal + (b.normal - a.normal) * u);
    result.uv = a.uv + (b.uv - a.uv) * u;

    return result;
}

std::vector<Vertex> Clipper::SutherlandHodgman(const std::vector<Vertex>& input, const Vec4& plane)
{
    std::vector<Vertex> output;

    if (input.empty()) 
        return output;

    Vertex p1 = input.back();

    for (const Vertex& p2 : input)
    {
        bool is_p1_inside = IsVertexInside(p1, plane);
        bool is_p2_inside = IsVertexInside(p2, plane);

        if ( is_p1_inside == true && is_p2_inside == true )
            output.push_back(p2);

        else if ( is_p1_inside == true && is_p2_inside == false )
            output.push_back(GetIntersectVertex(plane, p1, p2));

        else if ( is_p1_inside == false && is_p2_inside == true )
        {
            output.push_back(GetIntersectVertex(plane, p1, p2));
            output.push_back(p2);
        }

        p1 = p2;
    }

    return output;
}

Mesh Clipper::ClipMesh(const Mesh& mesh) const
{
    Mesh output;

    output.vertices.reserve(mesh.vertices.size());
    output.indices.reserve(mesh.indices.size());

    for (size_t i = 0; i + 2 < mesh.indices.size(); i += 3)
    {
        std::vector<Vertex> poly = {
            mesh.vertices[mesh.indices[i + 0]],
            mesh.vertices[mesh.indices[i + 1]],
            mesh.vertices[mesh.indices[i + 2]]
        };

        for (const Vec4& plane : frustum_planes)
        {
            poly = SutherlandHodgman(poly, const_cast<Vec4&>(plane));
            if (poly.empty()) break;
        }

        if (poly.size() < 3) continue;

        uint32_t baseIdx = static_cast<uint32_t>(output.vertices.size());
        for (const auto& v : poly)
            output.vertices.push_back(v);

        for (size_t j = 1; j + 1 < poly.size(); ++j)
        {
            output.indices.push_back(baseIdx + 0);
            output.indices.push_back(baseIdx + j);
            output.indices.push_back(baseIdx + j + 1);
        }
    }

    return output;
}

bool Clipper::IsAABBVisible(const AABB& box) const
{
    for (const auto& p : frustum_planes)
    {
        float dist = p.x * ((p.x >= 0.0f) ? box.max.x : box.min.x)
                   + p.y * ((p.y >= 0.0f) ? box.max.y : box.min.y)
                   + p.z * ((p.z >= 0.0f) ? box.max.z : box.min.z)
                   + p.w;
                   
        if ( dist < 0.0f )
            return false;
    }
    return true;
}


/*
    | Xh |   |     row 0     |     | X | 
    | Yh | = |     row 1     |  *  | Y |
    | Zh |   |     row 2     |     | Z |
    | Wh |   |     row 3     |     | 1 |

    Wh + Xh >= 0 ... row 3 + row 0
    Wh - Xh >= 0 ... row 3 - row 0
    Wh + Yh >= 0 ... row 3 + row 1
    Wh - Yh >= 0 ... row 3 - row 1
    Wh + Zh >= 0 ... row 3 + row 2
    Wh - Zh >= 0 ... row 3 - row 2

    Maybe, it can be made more clean code
    but, I think performance is more important than readability 
*/
void Clipper::ExtractFrustumPlanes(const Mat4x4& proj_view)
{
    frustum_planes[0] = Vec4(
        proj_view.m[3][0] + proj_view.m[0][0],
        proj_view.m[3][1] + proj_view.m[0][1],
        proj_view.m[3][2] + proj_view.m[0][2],
        proj_view.m[3][3] + proj_view.m[0][3]
    );
    frustum_planes[1] = Vec4(
        proj_view.m[3][0] - proj_view.m[0][0],
        proj_view.m[3][1] - proj_view.m[0][1],
        proj_view.m[3][2] - proj_view.m[0][2],
        proj_view.m[3][3] - proj_view.m[0][3]
    );
    frustum_planes[2] = Vec4(
        proj_view.m[3][0] + proj_view.m[1][0],
        proj_view.m[3][1] + proj_view.m[1][1],
        proj_view.m[3][2] + proj_view.m[1][2],
        proj_view.m[3][3] + proj_view.m[1][3]
    );
    frustum_planes[3] = Vec4(
        proj_view.m[3][0] - proj_view.m[1][0],
        proj_view.m[3][1] - proj_view.m[1][1],
        proj_view.m[3][2] - proj_view.m[1][2],
        proj_view.m[3][3] - proj_view.m[1][3]
    );
    frustum_planes[4] = Vec4(
        proj_view.m[3][0] + proj_view.m[2][0],
        proj_view.m[3][1] + proj_view.m[2][1],
        proj_view.m[3][2] + proj_view.m[2][2],
        proj_view.m[3][3] + proj_view.m[2][3]
    );
    frustum_planes[5] = Vec4(
        proj_view.m[3][0] - proj_view.m[2][0],
        proj_view.m[3][1] - proj_view.m[2][1],
        proj_view.m[3][2] - proj_view.m[2][2],
        proj_view.m[3][3] - proj_view.m[2][3]
    );

    for (auto& p : frustum_planes) {
        float len = std::sqrt(p.x*p.x + p.y*p.y + p.z*p.z);
        p /= len;
    }
}

Mesh Clipper::BackFaceRemoval(const Mesh& in, const Vec3& view_direction) const
{
    Mesh out;
    out.material = in.material;

    out.vertices = in.vertices;
    out.indices.reserve(in.indices.size());

    const auto triCount = in.indices.size() / 3;
    for (size_t t = 0; t < triCount; ++t)
    {
        uint32_t i0 = in.indices[3*t + 0];
        uint32_t i1 = in.indices[3*t + 1];
        uint32_t i2 = in.indices[3*t + 2];

        Vec3 normal_avg = in.vertices[i0].normal + 
                          in.vertices[i1].normal + 
                          in.vertices[i2].normal;

        if (Vec3::Dot( normal_avg, view_direction) < 0.0f)
        {
            out.indices.push_back(i0);
            out.indices.push_back(i1);
            out.indices.push_back(i2);
        }
    }

    return out;
}

Mesh Clipper::BackFaceRemoval2(const Mesh& in, const Mat4x4& V) const
{
    Mesh out;
    out.material = in.material;
    out.vertices = in.vertices;
    out.indices.reserve(in.indices.size());
    Mat3x3 V_topleft = V.TopLeft3x3();

    const auto triCount = in.indices.size() / 3;
    for (size_t t = 0; t < triCount; ++t)
    {
        const uint32_t i0 = in.indices[3*t + 0];
        const uint32_t i1 = in.indices[3*t + 1];
        const uint32_t i2 = in.indices[3*t + 2];

        const Vec3 n0 = V_topleft * in.vertices[i0].normal;
        const Vec3 n1 = V_topleft * in.vertices[i1].normal;
        const Vec3 n2 = V_topleft * in.vertices[i2].normal;
        const Vec3 n_avg = Vec3::Normalize( n0 + n1 + n2 );

        if ( n_avg.z < 0.0f )
        {
            out.indices.push_back(i0);
            out.indices.push_back(i1);
            out.indices.push_back(i2);
        }
    }
    return out;
}

Mesh Clipper::BackFaceCull_View(const Mesh& in, const Mat4x4& V) const
{
    Mesh out;
    out.material = in.material;
    out.vertices = in.vertices;
    out.indices.reserve(in.indices.size());

    const auto triCount = in.indices.size() / 3;
    for (size_t t = 0; t < triCount; ++t)
    {
        const uint32_t i0 = in.indices[3*t + 0];
        const uint32_t i1 = in.indices[3*t + 1];
        const uint32_t i2 = in.indices[3*t + 2];

        const Vec3 p0v = (V * in.vertices[i0].position).ToVec3();
        const Vec3 p1v = (V * in.vertices[i1].position).ToVec3();
        const Vec3 p2v = (V * in.vertices[i2].position).ToVec3();

        const Vec3 e1 = p1v - p0v;
        const Vec3 e2 = p2v - p0v;
        const Vec3 n  = Vec3::Cross(e1, e2);

        const Vec3 to_eye = -p0v;

        if (Vec3::Dot(n, to_eye) > 0.0f)
        {
            out.indices.push_back(i0);
            out.indices.push_back(i1);
            out.indices.push_back(i2);
        }
    }
    return out;
}

Mesh Clipper::BackFaceCull_View2(const Mesh& mesh, const Vec3& camera_pos) const
{
    Mesh out;
    out.material = mesh.material;
    out.vertices = mesh.vertices;
    out.indices.reserve(mesh.indices.size());

    const auto triCount = mesh.indices.size() / 3;

    for (size_t t = 0; t < triCount; ++t)
    {
        const uint32_t i0 = mesh.indices[3*t + 0];
        const uint32_t i1 = mesh.indices[3*t + 1];
        const uint32_t i2 = mesh.indices[3*t + 2];

        const Vec3 e1 = (mesh.vertices[i1].position - mesh.vertices[i0].position).ToVec3();
        const Vec3 e2 = (mesh.vertices[i2].position - mesh.vertices[i0].position).ToVec3();
        const Vec3 n  = Vec3::Cross(e1, e2);

        const Vec3 to_camera_pos = camera_pos-mesh.vertices[i0].position.ToVec3();

        if (Vec3::Dot(n, to_camera_pos) > 0.0f)
        {
            out.indices.push_back(i0);
            out.indices.push_back(i1);
            out.indices.push_back(i2);
        }
    }
    return out;
}