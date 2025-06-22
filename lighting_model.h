#pragma once

#include "vector.h"
#include "light.h"
#include <vector>
#include <memory>

class Lighting_Model
{
public:
    virtual ~Lighting_Model() = default;

    virtual Color Shade(
        const Vec3& P,
        const Vec3& N,
        const Vec3& V,
        const std::vector<std::shared_ptr<Light>>& lights
    ) const = 0;
};