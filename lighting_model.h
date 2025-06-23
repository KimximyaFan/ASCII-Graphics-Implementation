#pragma once

#include "vector.h"
#include "light.h"
#include <vector>
#include <memory>
#include "material.h"

class Lighting_Model
{
public:
    virtual ~Lighting_Model() = default;

    virtual float Shade(
        const Vec3& P,
        const Vec3& N,
        const Vec3& V,
        const Material& material,
        const std::vector<std::shared_ptr<Light>>& lights
    ) const = 0;

protected:
    Lighting_Model() = default;
};