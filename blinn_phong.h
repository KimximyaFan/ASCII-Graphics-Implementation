#pragma once

#include "lighting_model.h"

class Blinn_Phong : public Lighting_Model
{
public:
    float Shade(
        const Vec3& P,
        const Vec3& N,
        const Vec3& V,
        const Material& material,
        const std::vector<std::shared_ptr<Light>>& lights
    ) const override
    {
        float intensity, N_L, N_H;
        float ambient, diffuse, specular;
        float result = 0.0f;

        Vec3 normalize_N = Vec3::Normalize(N);
        Vec3 normalize_V = Vec3::Normalize(V);

        for (auto& light : lights)
        {
            intensity = light->GetIntensity(P);
            Vec3 normalized_L = Vec3::Normalize(light->GetDirection(P));
            Vec3 normalized_H = Vec3::Normalize(normalized_L+normalize_V);

            ambient = material.ambient_coef * intensity;

            N_L = std::max(0.0f, Vec3::Dot(normalize_N, normalized_L));
            diffuse = material.diffuse_coef * intensity * N_L;

            N_H = std::max(0.0f, Vec3::Dot(normalize_N, normalized_H));
            specular = material.specular_coef * intensity * std::powf(N_H, material.shininess);
            
            result += ambient + diffuse + specular;
        }

        return result;
    }
};