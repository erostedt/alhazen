#pragma once

#include "color.hpp"
#include "hit_payload.hpp"
#include "ray.hpp"
#include "texture.hpp"

struct ScatterPayload
{
    Ray Scattered;
    Color Attenuation = BLACK;
    Color Emitted = BLACK;
};

struct Material;
typedef ScatterPayload (*ScatterFunction)(const Material &material, const Ray &incoming_ray, const HitPayload &hit);

namespace MaterialTypes
{
struct Lambertian
{
    Texture Tex;
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

struct DiffuseLight
{
    Texture Tex;
};

} // namespace MaterialTypes

struct Material
{
    union {
        MaterialTypes::Lambertian Lambertian;
        MaterialTypes::Metal Metal;
        MaterialTypes::Dielectric Dielectric;
        MaterialTypes::DiffuseLight DiffuseLight;
    };
    ScatterFunction Scatter;
};

Material CreateLambertian(const Texture &texture);
Material CreateLambertian(Color albedo);
Material CreateMetal(Color albedo, f32 fuzz_factor);
Material CreateDielectric(f32 refractive_index);
Material CreateDiffuseLighting(Color albedo);
