
#include "clipper.h"
#include <iostream>

void Clipper::SetFrustumPlanes(const std::array<Vec4, 6>& planes)
{
    frustum_planes = planes;
}

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
    result.color.r = a.color.r + (b.color.r - a.color.r) * u;
    result.color.g = a.color.g + (b.color.g - a.color.g) * u; 
    result.color.b = a.color.b + (b.color.b - a.color.b) * u;
    result.color.a = a.color.a + (b.color.a - a.color.a) * u;
    // 필요시 나머지 속성도 동일하게 보간
    // 예) normal
    // Vec3 dn = b.normal - a.normal;
    // result.normal = Vec3::Normalize(a.normal + dn * t);
    // 예) uv
    // result.uv = a.uv + (b.uv - a.uv) * t;
    // 예) color
    // result.color = a.color + (b.color - a.color) * t;

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