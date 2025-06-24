#pragma once

#include "vector.h"
#include "color.h"

enum class Light_Type
{
    DIRECTIONAL,
    POINT
};

class Light
{
public:
    virtual ~Light() = default;

    virtual Light_Type GetType() const = 0;

    virtual Vec3 GetDirection(const Vec3& P) const = 0;

    virtual float GetIntensity(const Vec3& P) const = 0;

protected:
    Light() = default;
};