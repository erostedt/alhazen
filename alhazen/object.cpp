#include "object.hpp"
#include "box.hpp"
#include "hit_payload.hpp"
#include "interval.hpp"
#include "random.hpp"
#include "vec3.hpp"

#include <cassert>
#include <cmath>
#include <limits>

using namespace ObjectTypes;

inline Vec3 SphereNormal(const Sphere &sphere, const Point3 &hit, f32 time) noexcept
{
    return (hit - sphere.PositionAt(time)) / sphere.Radius;
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
    Vec3 oc = sphere.PositionAt(ray.Time) - ray.Origin;
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

    bool outside = std::abs(alpha) > 0.5f || std::abs(beta) > 0.5f;

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
    payload.UVCoordinates = {alpha + 0.5f, beta + 0.5f};

    return true;
}

static bool ConstantMediumHit(const Object &object, const Ray &ray, Interval interval, HitPayload &payload)
{
    HitPayload payload1, payload2;

    Object *boundary = object.ConstantMedium.Boundary;
    if (!boundary->Hit(*boundary, ray, Interval::Universe(), payload1))
    {
        return false;
    }

    if (!boundary->Hit(*boundary, ray, {payload1.Distance + 0.0001f, std::numeric_limits<f32>::infinity()}, payload2))
    {
        return false;
    }

    payload1.Distance = std::max(payload1.Distance, interval.LowerBound);
    payload2.Distance = std::min(payload2.Distance, interval.UpperBound);

    if (payload1.Distance >= payload2.Distance)
    {
        return false;
    }

    payload1.Distance = std::max(payload1.Distance, 0.0f);

    f32 distance_inside_boundary = payload2.Distance - payload1.Distance;
    auto hit_distance = object.ConstantMedium.NegativeInverseDensity * std::log(UniformF32());

    if (hit_distance > distance_inside_boundary)
    {
        return false;
    }

    payload.Distance = payload1.Distance + hit_distance;
    payload.Position = ray.At(payload.Distance);
    payload.Normal = RIGHT;
    payload.FrontFacing = true;
    payload.UVCoordinates = {0.0f, 0.0f};
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

Object CreateQuad(Point3 center, Vec3 u, Vec3 v, u32 material_index)
{
    Quad quad;
    quad.Anchor = center;
    quad.U = u;
    quad.V = v;

    Vec3 n = Cross(u, v);
    quad.Normal = Normalized(n);
    quad.D = Dot(quad.Normal, ToVec3(quad.Anchor));

    Object obj;
    obj.Quad = quad;
    obj.MaterialIndex = material_index;

    Point3 p0 = center - 0.5f * u - 0.5f * v;
    Point3 p1 = center + 0.5f * u - 0.5f * v;
    Point3 p2 = center + 0.5f * u + 0.5f * v;
    Point3 p3 = center - 0.5f * u + 0.5f * v;

    f32 xmin = std::min(std::min(std::min(p0.X, p1.X), p2.X), p3.X);
    f32 ymin = std::min(std::min(std::min(p0.Y, p1.Y), p2.Y), p3.Y);
    f32 zmin = std::min(std::min(std::min(p0.Z, p1.Z), p2.Z), p3.Z);

    f32 xmax = std::max(std::max(std::max(p0.X, p1.X), p2.X), p3.X);
    f32 ymax = std::max(std::max(std::max(p0.Y, p1.Y), p2.Y), p3.Y);
    f32 zmax = std::max(std::max(std::max(p0.Z, p1.Z), p2.Z), p3.Z);

    Box box = CreateBox({xmin, ymin, zmin}, {xmax, ymax, zmax});

    f32 min_width = 0.0001f;
    obj.BoundingBox = ExpandToAtleast(box, min_width);
    obj.Hit = QuadHit;

    return obj;
}

Object CreateConstantMedium(Object *object, f32 density, u32 material_index)
{
    ConstantMedium medium;
    medium.Boundary = object;
    medium.NegativeInverseDensity = -1.0f / density;
    Object obj;
    obj.ConstantMedium = medium;
    obj.MaterialIndex = material_index;
    obj.BoundingBox = object->BoundingBox;
    obj.Hit = ConstantMediumHit;
    return obj;
}
