#pragma once

#include "color.hpp"
#include "hit_payload.hpp"
#include "ray.hpp"

struct ScatterPayload
{
    Ray Scattered;
    Color Attenuation;
    bool Absorbed = false;
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

ScatterPayload Scatter(Ray incoming_ray, const HitPayload &hit, Material material);
