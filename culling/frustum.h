#pragma once

#include "math/vector.h"
#include "math/matrix.h"
#include <array>

class Frustum
{
public:
    Frustum () {}
    void ExtractFrustumPlanes(const Mat4x4& proj_view);
    const std::array<Vec4, 6>& GetFrustumPlanes() const;

private:
    std::array<Vec4, 6> frustum_planes;
};