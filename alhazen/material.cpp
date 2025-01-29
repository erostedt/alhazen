#include "material.hpp"
#include "color.hpp"
#include "random.hpp"
#include "ray.hpp"
#include "vec3.hpp"
#include <algorithm>
#include <cassert>
#include <cmath>

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

ScatterPayload Scatter(Ray incoming_ray, HitPayload hit, Material material)
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
        assert(0 && "Unexpected");
    }
}

ScatterPayload Scatter(HitPayload hit, Lambertian material)
{
    ScatterPayload payload;

    Vec3 direction = hit.Normal + RandomUnitVector();
    if (NearZero(direction))
    {
        direction = hit.Normal;
    }

    payload.Scattered = {hit.Position, direction};
    payload.Attenuation = material.Albedo;
    return payload;
}

static Vec3 Reflect(Vec3 v, Vec3 normal)
{
    return v - 2.0f * Dot(v, normal) * normal;
}

ScatterPayload Scatter(Ray incoming_ray, HitPayload hit, Metal material)
{
    ScatterPayload payload;
    Vec3 fuzziness = material.FuzzFactor * RandomUnitVector();
    Vec3 direction = Normalized(Reflect(incoming_ray.Direction, hit.Normal) + fuzziness);
    payload.Scattered = {hit.Position, direction};
    payload.Attenuation = material.Albedo;
    payload.Absorbed = Dot(direction, hit.Normal) < 0;
    return payload;
}

static Vec3 Refract(Vec3 v, Vec3 normal, f32 relative_refractive_index)
{
    f32 cos_theta = std::clamp(Dot(-v, normal), -1.0f, 1.0f);
    Vec3 perpendicular = relative_refractive_index * (v + cos_theta * normal);
    Vec3 parallel = -std::sqrt(std::fabs(1.0f - SquaredLength(perpendicular))) * normal;
    return perpendicular + parallel;
}

static bool TotalInternalReflection(Vec3 v, Vec3 normal, f32 refractive_index)
{
    f32 cos_theta = std::clamp(Dot(-v, normal), -1.0f, 1.0f);
    f32 sin_theta = std::sqrt(1.0f - cos_theta * cos_theta);
    return (sin_theta * refractive_index) > 1.0f;
}

static f32 Reflectance(f32 refractive_index, f32 cosine)
{
    f32 r0 = (1.0f - refractive_index) / (1.0f + refractive_index);
    r0 = r0 * r0;
    return r0 + (1.0f - r0) * std::pow((1.0f - cosine), 5.0f);
}

ScatterPayload Scatter(Ray incoming_ray, HitPayload hit, Dielectric material)
{
    ScatterPayload payload;
    payload.Attenuation = Color{1.0f, 1.0f, 1.0f};

    Vec3 normal = hit.Normal;
    f32 refractive_index = 1.0f / material.RefractiveIndex;

    if (!FrontFacing(incoming_ray, hit.Normal))
    {
        normal = -hit.Normal;
        refractive_index = 1.0f / refractive_index;
    }

    f32 cosine = std::clamp(Dot(-incoming_ray.Direction, normal), -1.0f, 1.0f);

    bool should_reflect = TotalInternalReflection(incoming_ray.Direction, normal, refractive_index) ||
                          Reflectance(refractive_index, cosine) > UniformF32();

    Vec3 direction = should_reflect ? Reflect(incoming_ray.Direction, normal)
                                    : Refract(incoming_ray.Direction, normal, refractive_index);

    payload.Scattered = Ray(hit.Position, direction);
    return payload;
}
