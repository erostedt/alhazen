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

struct Material;
typedef ScatterPayload (*ScatterFunction)(const Material &material, const Ray &incoming_ray, const HitPayload &hit);

namespace MaterialTypes
{
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
} // namespace MaterialTypes

struct Material
{
    union {
        MaterialTypes::Lambertian Lambertian;
        MaterialTypes::Metal Metal;
        MaterialTypes::Dielectric Dielectric;
    };
    ScatterFunction Scatter;
};

Material CreateLambertian(Color albedo);
Material CreateMetal(Color albedo, f32 fuzz_factor);
Material CreateDielectric(f32 refractive_index);
