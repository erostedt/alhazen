#include <cmath>
#include <iostream>

#include "color.hpp"
#include "image.hpp"
#include "ray.hpp"
#include "vec3.hpp"

#define internal static

internal f32 HitSphere(const Vec3 &center, f32 radius, const Ray &r)
{
    Vec3 oc = center - r.Origin;
    f32 a = Dot(r.Direction, r.Direction);
    f32 b = -2.0f * Dot(r.Direction, oc);
    f32 c = Dot(oc, oc) - radius * radius;
    auto discriminant = b * b - 4 * a * c;
    if (discriminant < 0.0f)
    {
        return -1.0f;
    }
    return (-b - std::sqrt(discriminant)) / (2.0f * a);
}

internal Color Vec3ToColor(Vec3 v)
{
    return {v.X, v.Y, v.Z};
}

internal Color RayColor(const Ray &r)
{
    Vec3 sphere_center = {0, 0, -1};
    f32 radius = 0.5f;
    f32 distance = HitSphere(sphere_center, radius, r);
    if (distance >= 0.0f)
    {
        Vec3 normal = r.At(distance) - sphere_center;
        return Vec3ToColor(0.5f * (normal + 1.0f));
    }

    Vec3 v = Normalized(r.Direction);
    f32 a = 0.5f * (v.Y + 1.0f);
    Color light_blue = {0.5f, 0.7f, 1.0f};
    Color white = {1.0f, 1.0f, 1.0f};

    return LinearBlend(light_blue, white, a);
}

int main()
{
    constexpr f32 ideal_aspect_ratio = 16.0f / 9.0f;
    u32 image_width = 400;
    u32 image_height = (u32)((f32)image_width / ideal_aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    f32 actual_aspect_ratio = (f32)image_width / (f32)image_height;

    f32 focal_length = 1.0f;
    f32 viewport_height = 2.0f;
    f32 viewport_width = viewport_height * actual_aspect_ratio;

    const Vec3 camera_center = {};
    const Vec3 viewport_u = {viewport_width, 0.0f, 0.0f};
    const Vec3 viewport_v = {0.0f, -viewport_height, 0.0f};

    const Vec3 pixel_delta_u = viewport_u / (f32)image_width;
    const Vec3 pixel_delta_v = viewport_v / (f32)image_height;

    const Vec3 viewport_upper_left = camera_center - Vec3{0, 0, focal_length} - viewport_u / 2 - viewport_v / 2;
    const Vec3 pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    FloatImage image = AllocateFloatImage(image_width, image_height);
    for (u32 y = 0; y < image.Height; ++y)
    {
        for (u32 x = 0; x < image.Width; ++x)
        {
            const Vec3 pixel_center = pixel00_loc + ((f32)x * pixel_delta_u) + ((f32)y * pixel_delta_v);
            const Vec3 ray_direction = pixel_center - camera_center;
            Ray ray = {camera_center, ray_direction};
            image[x, y] = RayColor(ray);
        }
    }
    WriteFloatImageToPPM(std::cout, image);
}
