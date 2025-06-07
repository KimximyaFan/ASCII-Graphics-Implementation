#pragma once

#include "clipper.h"

Clipper::Clipper(const Mat4x4& perspective_mat) 
: proj_view(perspective_mat), 
frustum_planes(ExtractFrustumPlanes(proj_view)) {}

std::array<Vec4, 6> Clipper::ExtractFrustumPlanes(const Mat4x4& projView)
{

}

bool Clipper::IsVertexInside(const Vertex& vertex, const Vec4& plane)
{

}

Vertex Clipper::GetIntersectVertex(const Vec4& plane, const Vertex& a, const Vertex& b)
{

}

std::vector<Vertex> Clipper::SutherlandHodgmanClipping(std::vector<Vertex> polygon, Vec4& plane)
{

}