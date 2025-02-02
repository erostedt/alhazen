#include "material.hpp"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <utility>

#include "color.hpp"
#include "random.hpp"
#include "ray.hpp"
#include "vec3.hpp"

Material CreateLambertian(Color albedo)
{
    Lambertian lambertian;
    lambertian.Albedo = albedo;

    Material material;
    material.Type = MaterialType::LAMBERTIAN;
    material.L = lambertian;
    return material;
}

Material CreateMetal(Color albedo, f32 fuzz_factor)
{
    Metal metal;
    metal.Albedo = albedo;
    metal.FuzzFactor = fuzz_factor;

    Material material;
    material.Type = MaterialType::METAL;
    material.M = metal;
    return material;
}

Material CreateDielectric(f32 refractive_index)
{
    Dielectric dielectric;
    dielectric.RefractiveIndex = refractive_index;

    Material material;
    material.Type = MaterialType::DIELECTRIC;
    material.D = dielectric;
    return material;
}

inline Vec3 Reflect(Vec3 v, Vec3 normal)
{
    return v - 2.0f * Dot(v, normal) * normal;
}

inline Vec3 Refract(f32 cosine, Vec3 v, Vec3 normal, f32 relative_refractive_index)
{
    Vec3 perpendicular = relative_refractive_index * (v + cosine * normal);
    Vec3 parallel = -std::sqrt(std::fabs(1.0f - SquaredLength(perpendicular))) * normal;
    return perpendicular + parallel;
}

inline bool TotalInternalReflection(f32 cosine, f32 refractive_index)
{
    f32 sin_theta = std::sqrt(1.0f - cosine * cosine);
    return (sin_theta * refractive_index) > 1.0f;
}

inline f32 Reflectance(f32 refractive_index, f32 cosine)
{
    f32 r0 = (1.0f - refractive_index) / (1.0f + refractive_index);
    r0 = r0 * r0;
    return r0 + (1.0f - r0) * std::pow(1.0f - cosine, 5.0f);
}

ScatterPayload Scatter(const HitPayload &hit, Lambertian material)
{
    ScatterPayload payload;

    Vec3 direction = Normalized(hit.Normal) + RandomUnitVector();
    if (NearZero(direction))
    {
        direction = hit.Normal;
    }

    payload.Scattered = {hit.Position, Normalized(direction)};
    payload.Attenuation = material.Albedo;
    return payload;
}

ScatterPayload Scatter(Ray incoming_ray, const HitPayload &hit, Metal material)
{
    ScatterPayload payload;
    Vec3 fuzziness = material.FuzzFactor * RandomUnitVector();
    Vec3 direction = Reflect(incoming_ray.Direction, hit.Normal) + fuzziness;
    payload.Scattered = {hit.Position, Normalized(direction)};
    payload.Attenuation = material.Albedo;
    payload.Absorbed = Dot(direction, hit.Normal) < 0.0f;
    return payload;
}

ScatterPayload Scatter(Ray incoming_ray, const HitPayload &hit, Dielectric material)
{
    ScatterPayload payload;
    payload.Attenuation = WHITE;

    Vec3 normal = hit.Normal;
    f32 refractive_index = hit.FrontFacing ? (1.0f / material.RefractiveIndex) : material.RefractiveIndex;

    f32 cosine = std::clamp(Dot(-incoming_ray.Direction, normal), -1.0f, 1.0f);

    bool should_reflect =
        TotalInternalReflection(cosine, refractive_index) || Reflectance(refractive_index, cosine) > UniformF32();

    Vec3 direction = should_reflect ? Reflect(incoming_ray.Direction, normal)
                                    : Refract(cosine, incoming_ray.Direction, normal, refractive_index);

    payload.Scattered = Ray{hit.Position, Normalized(direction)};
    return payload;
}

ScatterPayload Scatter(Ray incoming_ray, const HitPayload &hit, Material material)
{
    switch (material.Type)
    {
    case MaterialType::LAMBERTIAN: {
        return Scatter(hit, material.L);
    }
    case MaterialType::METAL: {
        return Scatter(incoming_ray, hit, material.M);
    }
    case MaterialType::DIELECTRIC: {
        return Scatter(incoming_ray, hit, material.D);
    }
    }
    std::unreachable();
}
