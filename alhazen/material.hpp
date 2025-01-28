#pragma once

#include "color.hpp"
#include "hit_payload.hpp"
#include "ray.hpp"

struct ScatterPayload
{
    Ray Scattered;
    Color Attenuation;
};

struct Lambertian
{
    Color Albedo;
};

struct Metal
{
    Color Albedo;
    f32 FuzzFactor;
};

struct Dielectric
{
    f32 RefractiveIndex;
};

enum class MaterialType
{
    LAMBERTIAN,
    METAL,
    DIELECTRIC,
};

struct Material
{
    MaterialType Type;
    union {
        Lambertian L;
        Metal M;
        Dielectric D;
    };
};

Material CreateLambertian(Color albedo);
Material CreateMetal(Color albedo, f32 fuzz_factor);
Material CreateDielectric(f32 refractive_index);

ScatterPayload Scatter(Ray incoming_ray, HitPayload hit, Material material);
ScatterPayload Scatter(HitPayload hit, Lambertian material);
ScatterPayload Scatter(Ray incoming_ray, HitPayload hit, Metal material);
