
#include "blinn_phong.h"

Color Blinn_Phong::Shade(
        const Vec3& P,
        const Vec3& N,
        const Vec3& V,
        const Material& material,
        const std::vector<std::shared_ptr<Light>>& lights,
        const Color& vertex_color,
        const Vec3& ambient_intensity
    ) const
{
    Color result_color;
    float intensity, N_L, N_H;
    float diffuse, specular;
    float result_diffuse, result_specular;

    Vec3 normalize_N = Vec3::Normalize(N);
    Vec3 normalize_V = Vec3::Normalize(V);

    for (auto& light : lights)
    {
        intensity = light->GetIntensity(P);
        Vec3 normalized_L = Vec3::Normalize(light->GetDirection(P));
        Vec3 normalized_H = Vec3::Normalize(normalized_L+normalize_V);

        N_L = std::max(0.0f, Vec3::Dot(normalize_N, normalized_L));
        diffuse = material.diffuse_coef * intensity * N_L;

        N_H = std::max(0.0f, Vec3::Dot(normalize_N, normalized_H));
        specular = material.specular_coef * intensity * std::powf(N_H, material.shininess);
        
        result_diffuse += diffuse;
        result_specular += specular;
    }

    result_color.r = vertex_color.r*(result_diffuse + material.ambient_coef*ambient_intensity.x) + result_specular;
    result_color.g = vertex_color.g*(result_diffuse + material.ambient_coef*ambient_intensity.y) + result_specular;
    result_color.b = vertex_color.b*(result_diffuse + material.ambient_coef*ambient_intensity.z) + result_specular;
    result_color.a = vertex_color.a;

    return result_color;
}