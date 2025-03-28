#include "material.hpp"

#include <algorithm>
#include <cassert>
#include <cmath>

#include "color.hpp"
#include "random.hpp"
#include "ray.hpp"
#include "texture.hpp"
#include "vec3.hpp"

using namespace MaterialTypes;

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

static ScatterPayload ScatterLambertian(const Material &material, const Ray &incoming_ray, const HitPayload &hit)
{
    (void)incoming_ray;
    ScatterPayload payload;

    Vec3 direction = Normalized(hit.Normal) + RandomUnitVector();
    if (NearZero(direction))
    {
        direction = hit.Normal;
    }

    payload.Scattered = {hit.Position, Normalized(direction), incoming_ray.Time};
    const Texture &texture = material.Lambertian.Tex;
    payload.Attenuation = texture.Sample(texture, hit.UVCoordinates, hit.Position);
    return payload;
}

static ScatterPayload ScatterMetal(const Material &material, const Ray &incoming_ray, const HitPayload &hit)
{
    ScatterPayload payload;
    const Metal &metal = material.Metal;
    Vec3 fuzziness = metal.FuzzFactor * RandomUnitVector();
    Vec3 direction = Reflect(incoming_ray.Direction, hit.Normal) + fuzziness;
    payload.Scattered = {hit.Position, Normalized(direction), incoming_ray.Time};
    payload.Attenuation = (Dot(direction, hit.Normal) < 0.0f) ? BLACK : metal.Albedo;
    return payload;
}

static ScatterPayload ScatterDielectric(const Material &material, const Ray &incoming_ray, const HitPayload &hit)
{
    ScatterPayload payload;
    payload.Attenuation = WHITE;

    Vec3 normal = hit.Normal;
    f32 refractive_index = material.Dielectric.RefractiveIndex;
    refractive_index = hit.FrontFacing ? (1.0f / refractive_index) : refractive_index;

    f32 cosine = std::clamp(Dot(-incoming_ray.Direction, normal), -1.0f, 1.0f);

    bool should_reflect =
        TotalInternalReflection(cosine, refractive_index) || Reflectance(refractive_index, cosine) > UniformF32();

    Vec3 direction = should_reflect ? Reflect(incoming_ray.Direction, normal)
                                    : Refract(cosine, incoming_ray.Direction, normal, refractive_index);

    payload.Scattered = {hit.Position, Normalized(direction), incoming_ray.Time};
    return payload;
}

static ScatterPayload ScatterDiffuseLighting(const Material &material, const Ray &incoming_ray, const HitPayload &hit)
{
    (void)incoming_ray;
    ScatterPayload payload;
    const Texture &texture = material.Lambertian.Tex;
    payload.Emitted = texture.Sample(texture, hit.UVCoordinates, hit.Position);
    return payload;
}

static ScatterPayload ScatterIsotropic(const Material &material, const Ray &incoming_ray, const HitPayload &hit)
{
    ScatterPayload payload;
    payload.Scattered = {hit.Position, RandomUnitVector(), incoming_ray.Time};
    const Texture &texture = material.Lambertian.Tex;
    payload.Attenuation = texture.Sample(texture, hit.UVCoordinates, hit.Position);
    return payload;
}

Material CreateLambertian(const Texture &texture)
{
    Lambertian lambertian;
    lambertian.Tex = texture;

    Material material;
    material.Lambertian = lambertian;
    material.Scatter = ScatterLambertian;
    return material;
}

Material CreateLambertian(Color albedo)
{
    return CreateLambertian(CreateSolidColor(albedo));
}

Material CreateMetal(Color albedo, f32 fuzz_factor)
{
    Metal metal;
    metal.Albedo = albedo;
    metal.FuzzFactor = fuzz_factor;

    Material material;
    material.Metal = metal;
    material.Scatter = ScatterMetal;
    return material;
}

Material CreateDielectric(f32 refractive_index)
{
    Dielectric dielectric;
    dielectric.RefractiveIndex = refractive_index;

    Material material;
    material.Dielectric = dielectric;
    material.Scatter = ScatterDielectric;
    return material;
}

Material CreateDiffuseLighting(Color albedo)
{
    DiffuseLight ligth;
    ligth.Tex = CreateSolidColor(albedo);

    Material material;
    material.DiffuseLight = ligth;
    material.Scatter = ScatterDiffuseLighting;
    return material;
}

Material CreateIsotropic(Color albedo)
{
    Isotropic isotropic;
    isotropic.Tex = CreateSolidColor(albedo);

    Material material;
    material.Isotropic = isotropic;
    material.Scatter = ScatterIsotropic;
    return material;
}
