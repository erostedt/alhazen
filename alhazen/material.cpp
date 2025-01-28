#include "material.hpp"
#include "color.hpp"
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
    Vec3 direction = Reflect(incoming_ray.Direction, hit.Normal) + fuzziness;
    payload.Scattered = {hit.Position, direction};
    payload.Attenuation = material.Albedo;
    return payload;
}
