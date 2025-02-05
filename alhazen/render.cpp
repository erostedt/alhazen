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

/*
static HitPayload TraceRay(const std::vector<Object> &objects, const Ray &r, Interval interval)
{
    i32 closest_object_index = -1;
    f32 closest_hit = std::numeric_limits<f32>::max();
    for (sz i = 0; i < objects.size(); ++i)
    {
        f32 hit = HitObject(objects[i], r, interval);
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

    Object obj = objects[(sz)closest_object_index];

    payload.Distance = closest_hit;
    payload.ObjectIndex = closest_object_index;
    payload.Position = r.At(closest_hit);
    payload.Normal = ObjectNormal(payload.Position, obj);
    payload.FrontFacing = FrontFacing(r, payload.Normal);
    if (!payload.FrontFacing)
    {
        payload.Normal = -payload.Normal;
    }
    return payload;
}
*/

static HitPayload TraceRayBVH(const BVH &bvh, const Ray &r, Interval interval)
{
    std::vector<u32> stack;
    stack.reserve(bvh.Objects.size());
    stack.push_back(0);

    i32 closest_object_index = -1;

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
            f32 hit = HitObject(obj, r, interval);
            if (interval.Surrounds(hit))
            {
                closest_object_index = (i32)node.ObjectIndex();
                interval.UpperBound = hit;
            }
            continue;
        }

        if (SquaredLength(bvh.Nodes[node.Left].BoundingBox.Center() - r.Origin) <
            SquaredLength(bvh.Nodes[node.Right].BoundingBox.Center() - r.Origin))
        {
            stack.push_back(node.Right);
            stack.push_back(node.Left);
        }
        else
        {
            stack.push_back(node.Left);
            stack.push_back(node.Right);
        }
    }

    HitPayload payload;
    if (closest_object_index < 0)
    {
        payload.ObjectIndex = -1;
        return payload;
    }

    const Object &obj = bvh.Objects[(sz)closest_object_index];

    payload.Distance = interval.UpperBound;
    payload.ObjectIndex = closest_object_index;
    payload.Position = r.At(interval.UpperBound);
    payload.Normal = ObjectNormal(payload.Position, obj);
    payload.FrontFacing = FrontFacing(r, payload.Normal);
    if (!payload.FrontFacing)
    {
        payload.Normal = -payload.Normal;
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

        u32 material_index = scene.Objects[(sz)hit.ObjectIndex].MaterialIndex;
        ScatterPayload scatter = Scatter(r, hit, scene.Materials[material_index]);
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
