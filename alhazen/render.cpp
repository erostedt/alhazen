#include "render.hpp"
#include "camera.hpp"
#include "color.hpp"
#include "hit_payload.hpp"
#include "image.hpp"
#include "interval.hpp"
#include "material.hpp"
#include "random.hpp"
#include "scene.hpp"
#include "types.hpp"
#include "vec3.hpp"
#include <limits>

f32 HitSphere(Sphere sphere, Ray r, Interval interval)
{
    Vec3 oc = sphere.Center - r.Origin;
    f32 a = SquaredLength(r.Direction);
    f32 h = Dot(r.Direction, oc);
    f32 c = SquaredLength(oc) - sphere.Radius * sphere.Radius;
    f32 discriminant = h * h - a * c;

    f32 miss = -1.0f;
    if (discriminant < 0.0f)
    {
        return miss;
    }

    f32 square_root_of_discriminant = std::sqrt(discriminant);
    f32 closest_hit = (h - square_root_of_discriminant) / a;
    if (interval.Surrounds(closest_hit))
    {
        return closest_hit;
    }

    f32 further_hit = (h + square_root_of_discriminant) / a;
    if (interval.Surrounds(further_hit))
    {
        return further_hit;
    }

    return miss;
}

HitPayload TraceRay(const std::vector<Sphere> &world, const Ray &r, Interval interval)
{
    i32 closest_object_index = -1;
    f32 closest_hit = std::numeric_limits<f32>::max();
    for (size_t i = 0; i < world.size(); ++i)
    {
        f32 hit = HitSphere(world[i], r, interval);
        if (interval.Surrounds(hit) && hit < closest_hit)
        {
            closest_object_index = (i32)i;
            closest_hit = hit;
        }
    }

    HitPayload payload;
    if (closest_object_index < 0)
    {
        payload.ObjectIndex = -1;
        return payload;
    }

    Sphere sphere = world[(size_t)closest_object_index];

    payload.Distance = closest_hit;
    payload.ObjectIndex = closest_object_index;
    payload.Position = r.At(closest_hit);
    payload.Normal = (payload.Position - sphere.Center) / sphere.Radius;
    return payload;
}

Color Vec3ToColor(Vec3 v)
{
    return {v.X, v.Y, v.Z};
}

Color RayColor(const Ray &r, const Scene &scene, u32 max_bounces)
{
    if (max_bounces == 0)
    {
        return BLACK;
    }

    Interval interval = {0.001f, std::numeric_limits<f32>::infinity()};
    const HitPayload hit = TraceRay(scene.Spheres, r, interval);
    if (hit.ObjectIndex >= 0)
    {
        u32 material_index = scene.Spheres[(size_t)hit.ObjectIndex].MaterialIndex;
        ScatterPayload scatter = Scatter(r, hit, scene.Materials[material_index]);
        return (scatter.Absorbed) ? BLACK : scatter.Attenuation * RayColor(scatter.Scattered, scene, max_bounces - 1);
    }

    Vec3 v = Normalized(r.Direction);
    f32 a = 0.5f * (v.Y + 1.0f);
    Color light_blue = {0.5f, 0.7f, 1.0f};
    Color white = {1.0f, 1.0f, 1.0f};

    return LinearBlend(light_blue, white, a);
}

Vec3 SampleUnitSquare()
{
    f32 x = UniformF32(-0.5f, 0.5f);
    f32 y = UniformF32(-0.5f, 0.5f);
    return {x, y, 0.0f};
}

FloatImage RenderImage(const Camera &camera, const Scene &scene, u32 rays_per_pixel, u32 max_bounces)
{
    FloatImage image = CreateFloatImage(camera.ImageResolution.Width, camera.ImageResolution.Height);
    for (u32 y = 0; y < image.Height; ++y)
    {
        for (u32 x = 0; x < image.Width; ++x)
        {
            Color c = BLACK;
            for (u32 s = 0; s < rays_per_pixel; ++s)
            {
                Vec3 offset = SampleUnitSquare();
                Ray ray = camera.GenerateRay((f32)x + offset.X, (f32)y + offset.Y);
                c += RayColor(ray, scene, max_bounces);
            }
            image[x, y] = c / (f32)rays_per_pixel;
        }
    }
    return image;
}
