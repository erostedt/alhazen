#include "render.hpp"

#include <iostream>
#include <limits>
#include <vector>

#include "box.hpp"
#include "bvh.hpp"
#include "camera.hpp"
#include "color.hpp"
#include "hit_payload.hpp"
#include "image.hpp"
#include "interval.hpp"
#include "material.hpp"
#include "object.hpp"
#include "random.hpp"
#include "ray.hpp"
#include "scene.hpp"
#include "types.hpp"
#include "vec3.hpp"

static HitPayload TraceRayBVH(const BVH &bvh, const Ray &r, Interval interval)
{
    std::vector<u32> stack;
    stack.reserve(bvh.Objects.size());
    stack.push_back(0);

    HitPayload payload = {};
    payload.ObjectIndex = -1;
    while (!stack.empty())
    {
        u32 node_index = stack.back();
        stack.pop_back();

        const BVHNode &node = bvh.Nodes[(sz)node_index];

        if (!HitsBox(node.BoundingBox, r, interval))
        {
            continue;
        }

        if (node.IsLeaf())
        {
            const Object &obj = bvh.Objects[(sz)node.ObjectIndex()];
            if (obj.Hit(obj, r, interval, payload))
            {
                payload.ObjectIndex = (i32)node.ObjectIndex();
                interval.UpperBound = payload.Distance;
            }
            continue;
        }

        stack.push_back(node.Left);
        stack.push_back(node.Right);
    }
    return payload;
}

static Color RayColor(Ray r, const Scene &scene, u32 max_bounces)
{
    Color color = WHITE;
    while (max_bounces > 0)
    {
        Interval interval = {0.001f, std::numeric_limits<f32>::infinity()};
        const HitPayload hit = TraceRayBVH(scene.Bvh, r, interval);
        if (hit.ObjectIndex < 0)
        {
            Vec3 v = r.Direction;
            f32 a = 0.5f * (v.Y + 1.0f);
            Color light_blue = {0.5f, 0.7f, 1.0f};
            Color white = {1.0f, 1.0f, 1.0f};
            return color * LinearBlend(light_blue, white, a);
        }

        u32 material_index = scene.Bvh.Objects[(sz)hit.ObjectIndex].MaterialIndex;
        const Material &material = scene.Materials[material_index];
        const ScatterFunction &Scatter = material.Scatter;
        ScatterPayload scatter = Scatter(material, r, hit);
        if (scatter.Absorbed)
        {
            return BLACK;
        }
        color = color * scatter.Attenuation;
        r = scatter.Scattered;
        --max_bounces;
    }

    return BLACK;
}

inline Vec3 SampleUnitSquare()
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
        std::clog << "\rScanlines remaining: " << (image.Height - y) << ' ' << std::flush;
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
