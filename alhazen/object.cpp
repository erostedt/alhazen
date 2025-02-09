#include "object.hpp"
#include "box.hpp"
#include "hit_payload.hpp"
#include "vec3.hpp"

#include <cassert>
#include <cmath>

using namespace ObjectTypes;

inline Vec3 SphereNormal(const Sphere &sphere, const Point3 &hit, f32 time) noexcept
{
    return (hit - sphere.CenterAt(time)) / sphere.Radius;
}

inline UV SphereUV(const Vec3 &normal)
{
    UV uv;

    constexpr f32 pi = std::numbers::pi_v<f32>;
    f32 theta = std::acos(-normal.Y);
    f32 phi = std::atan2(-normal.Z, normal.X) + pi;

    uv.U = phi / (2 * pi);
    uv.V = theta / pi;
    return uv;
}

static void FillSphereHitPayload(const Sphere &sphere, f32 distance, const Ray &ray, HitPayload &payload)
{
    payload.Distance = distance;
    payload.Position = ray.At(distance);
    payload.Normal = SphereNormal(sphere, payload.Position, ray.Time);
    payload.FrontFacing = FrontFacing(ray, payload.Normal);
    if (!payload.FrontFacing)
    {
        payload.Normal = -payload.Normal;
    }
    payload.UVCoordinates = SphereUV(payload.Normal);
}

static bool SphereHit(const Object &object, const Ray &ray, Interval interval, HitPayload &payload)
{
    const Sphere &sphere = object.Sphere;
    Vec3 oc = sphere.CenterAt(ray.Time) - ray.Origin;
    f32 h = Dot(ray.Direction, oc);
    f32 c = SquaredLength(oc) - sphere.Radius * sphere.Radius;
    f32 discriminant = h * h - c;

    if (discriminant < 0.0f)
    {
        return false;
    }

    f32 square_root_of_discriminant = std::sqrt(discriminant);
    f32 closest_hit = h - square_root_of_discriminant;
    if (interval.Surrounds(closest_hit))
    {
        FillSphereHitPayload(sphere, closest_hit, ray, payload);
        return true;
    }

    f32 further_hit = h + square_root_of_discriminant;
    if (interval.Surrounds(further_hit))
    {
        FillSphereHitPayload(sphere, further_hit, ray, payload);
        return true;
    }

    return false;
}

static bool QuadHit(const Object &object, const Ray &ray, Interval interval, HitPayload &payload)
{
    const Quad &quad = object.Quad;
    f32 denom = Dot(quad.Normal, ray.Direction);

    if (std::abs(denom) < 1e-6f)
    {
        return false;
    }

    f32 t = (quad.D - Dot(quad.Normal, ToVec3(ray.Origin))) / denom;

    if (!interval.Contains(t))
    {
        return false;
    }

    Point3 intersection = ray.At(t);

    Vec3 test_point = intersection - quad.Anchor;

    f32 alpha = Dot(test_point, quad.U) / SquaredLength(quad.U);
    f32 beta = Dot(test_point, quad.V) / SquaredLength(quad.V);

    bool outside = alpha < 0.0f || alpha > 1.0f || beta < 0.0f || beta > 1.0f;

    if (outside)
    {
        return false;
    }

    payload.Distance = t;
    payload.Position = intersection;
    payload.Normal = quad.Normal;
    payload.FrontFacing = FrontFacing(ray, payload.Normal);
    if (!payload.FrontFacing)
    {
        payload.Normal = -payload.Normal;
    }
    payload.UVCoordinates = {alpha, beta};

    return true;
}

Object CreateStationarySpere(Point3 center, f32 radius, u32 material_index)
{
    Sphere sphere = {center, center, radius};
    Object obj;
    obj.Sphere = sphere;
    obj.MaterialIndex = material_index;

    Vec3 delta = radius * ONE;
    obj.BoundingBox = CreateBox(center - delta, center + delta);
    obj.Hit = SphereHit;
    return obj;
}

Object CreateMovingSphere(Point3 start_center, Point3 end_center, f32 radius, u32 material_index)
{
    Sphere sphere = {start_center, end_center, radius};
    Object obj;
    obj.Sphere = sphere;
    obj.MaterialIndex = material_index;

    Vec3 delta = radius * ONE;
    Box start_box = CreateBox(start_center - delta, start_center + delta);
    Box end_box = CreateBox(end_center - delta, end_center + delta);
    obj.BoundingBox = Expand(start_box, end_box);
    obj.Hit = SphereHit;
    return obj;
}

Object CreateQuad(Point3 anchor, Vec3 u, Vec3 v, u32 material_index)
{

    Quad quad;
    quad.Anchor = anchor;
    quad.U = u;
    quad.V = v;

    Vec3 n = Cross(u, v);
    quad.Normal = Normalized(n);
    quad.D = Dot(quad.Normal, ToVec3(quad.Anchor));

    Object obj;
    obj.Quad = quad;
    obj.MaterialIndex = material_index;

    Box box_diag = CreateBox(anchor, anchor + u + v);
    Box box_off_diag = CreateBox(anchor + u, anchor + v);

    Box bounding_box = Expand(box_diag, box_off_diag);

    f32 min_width = 0.0001f;
    obj.BoundingBox = ExpandToAtleast(bounding_box, min_width);
    obj.Hit = QuadHit;

    return obj;
}
