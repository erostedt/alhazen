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
};

enum class MaterialType
{
    LAMBERTIAN,
    METAL
};

struct Material
{
    MaterialType Type;
    union {
        Lambertian L;
        Metal M;
    };
};

ScatterPayload Scatter(Ray incoming_ray, HitPayload hit, Material material);
ScatterPayload Scatter(HitPayload hit, Lambertian material);
ScatterPayload Scatter(Ray incoming_ray, HitPayload hit, Metal material);
