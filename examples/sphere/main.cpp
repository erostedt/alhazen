#include <cmath>
#include <iostream>
#include <limits>
#include <vector>

#include "color.hpp"
#include "hit_payload.hpp"
#include "image.hpp"
#include "interval.hpp"
#include "point3.hpp"
#include "ray.hpp"
#include "vec3.hpp"

#define internal static

struct Sphere
{
    Point3 Center;
    f32 Radius;
};

internal f32 HitSphere(Sphere sphere, Ray r, Interval interval)
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

    f32 closest_hit = (h - std::sqrt(discriminant)) / a;
    if (interval.Contains(closest_hit))
    {
        return closest_hit;
    }

    f32 further_hit = (h + std::sqrt(discriminant)) / a;
    if (interval.Contains(further_hit))
    {
        return further_hit;
    }

    return miss;
}

internal HitPayload TraceRay(const std::vector<Sphere> &world, const Ray &r, Interval interval)
{
    i32 closest_object_index = -1;
    f32 closest_hit = std::numeric_limits<f32>::max();
    for (size_t i = 0; i < world.size(); ++i)
    {
        f32 hit = HitSphere(world[i], r, interval);
        if (interval.Contains(hit) && hit < closest_hit)
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

internal Color Vec3ToColor(Vec3 v)
{
    return {v.X, v.Y, v.Z};
}

internal Color RayColor(const Ray &r)
{
    std::vector<Sphere> spheres{Sphere{{0, 0, -1}, 0.5f}, Sphere{{0, -100.5, -1}, 100.0f}};

    Interval interval = ZeroToInfinity();
    const HitPayload payload = TraceRay(spheres, r, interval);
    if (payload.ObjectIndex >= 0)
    {
        return Vec3ToColor(0.5f * (payload.Normal + 1.0f));
    }

    Vec3 v = Normalized(r.Direction);
    f32 a = 0.5f * (v.Y + 1.0f);
    Color light_blue = {0.5f, 0.7f, 1.0f};
    Color white = {1.0f, 1.0f, 1.0f};

    return LinearBlend(light_blue, white, a);
}

int main()
{
    f32 ideal_aspect_ratio = 16.0f / 9.0f;
    u32 image_width = 400;
    u32 image_height = (u32)((f32)image_width / ideal_aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    f32 actual_aspect_ratio = (f32)image_width / (f32)image_height;

    Vec3 forward = {0.0f, 0.0f, -1.0f};

    f32 focal_length = 1.0f;
    f32 viewport_height = 2.0f;
    f32 viewport_width = viewport_height * actual_aspect_ratio;

    Point3 camera_center = {};
    Vec3 viewport_u = {viewport_width, 0.0f, 0.0f};
    Vec3 viewport_v = {0.0f, -viewport_height, 0.0f};

    Vec3 pixel_delta_u = viewport_u / (f32)image_width;
    Vec3 pixel_delta_v = viewport_v / (f32)image_height;

    Point3 viewport_center = camera_center + focal_length * forward;

    Point3 viewport_upper_left = viewport_center - viewport_u / 2 - viewport_v / 2;
    Point3 pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    FloatImage image = AllocateFloatImage(image_width, image_height);
    for (u32 y = 0; y < image.Height; ++y)
    {
        for (u32 x = 0; x < image.Width; ++x)
        {
            const Point3 pixel_center = pixel00_loc + ((f32)x * pixel_delta_u) + ((f32)y * pixel_delta_v);
            const Vec3 ray_direction = pixel_center - camera_center;
            Ray ray = {camera_center, ray_direction};
            image[x, y] = RayColor(ray);
        }
    }
    WriteFloatImageToPPM(std::cout, image);
}
