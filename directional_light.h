#pragma once

#include "light.h"

class Directional_Light : public Light
{
public:
    Vec3 direction;
    float intensity;

    Directional_Light (const Vec3& dir, const float i)
        : direction(Vec3::Normalize(dir)), intensity(i) {}

    Light_Type GetType () const override
    {
        return Light_Type::DIRECTIONAL;
    }

    Vec3 GetDirection (const Vec3& ) const override
    {
        return -direction;
    }

    float GetIntensity (const Vec3& ) const override
    {
        return intensity;
    }
};