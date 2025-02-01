#include "object.hpp"
#include "interval.hpp"

#include <cassert>
#include <cmath>
#include <utility>

Object CreateSphere(Point3 center, f32 radius, u32 material_index)
{
    Sphere sphere{center, radius};
    Object obj;
    obj.Type = ObjectType::SPHERE;
    obj.S = sphere;
    obj.MaterialIndex = material_index;

    f32 padding = 1e-6f;
    Point3 top_left = center - (radius + padding) * ONE;
    Point3 bottom_right = center + (radius + padding) * ONE;

    obj.BoundingBox = CreateBox(top_left, bottom_right);
    return obj;
}

f32 HitSphere(const Sphere &sphere, const Ray &r, Interval interval)
{
    Vec3 oc = sphere.Center - r.Origin;
    f32 h = Dot(r.Direction, oc);
    f32 c = SquaredLength(oc) - sphere.Radius * sphere.Radius;
    f32 discriminant = h * h - c;

    f32 miss = -1.0f;
    if (discriminant < 0.0f)
    {
        return miss;
    }

    f32 square_root_of_discriminant = std::sqrt(discriminant);
    f32 closest_hit = h - square_root_of_discriminant;
    if (interval.Surrounds(closest_hit))
    {
        return closest_hit;
    }

    f32 further_hit = h + square_root_of_discriminant;
    if (interval.Surrounds(further_hit))
    {
        return further_hit;
    }

    return miss;
}

bool HitBox(const Box &box, const Ray &r, Interval interval)
{
    f32 hit1, hit2;
    hit1 = (box.X.LowerBound - r.Origin.X) / r.Direction.X;
    hit2 = (box.X.UpperBound - r.Origin.X) / r.Direction.X;
    interval = Overlap(interval, CreateInterval(hit1, hit2));
    if (interval.Empty())
    {
        return false;
    }

    hit1 = (box.Y.LowerBound - r.Origin.Y) / r.Direction.Y;
    hit2 = (box.Y.UpperBound - r.Origin.Y) / r.Direction.Y;
    interval = Overlap(interval, CreateInterval(hit1, hit2));
    if (interval.Empty())
    {
        return false;
    }

    hit1 = (box.Z.LowerBound - r.Origin.Z) / r.Direction.Z;
    hit2 = (box.Z.UpperBound - r.Origin.Z) / r.Direction.Z;
    interval = Overlap(interval, CreateInterval(hit1, hit2));
    if (interval.Empty())
    {
        return false;
    }

    return true;
}

f32 HitBVHNode(const BVHNode &node, const Ray &r, Interval interval)
{
    if (!HitBox(node.BoundingBox, r, interval))
    {
        return -1.0f;
    }

    f32 hit_left = HitBVHNode(*node.Left, r, interval);
    f32 hit_right = HitBVHNode(*node.Right, r, interval);

    if (hit_left < 0.0f)
    {
        return hit_right;
    }

    if (hit_right < 0.0f)
    {
        return hit_left;
    }

    return std::fmin(hit_left, hit_right);
}

f32 HitObject(const Object &obj, const Ray &r, Interval interval) noexcept
{
    switch (obj.Type)
    {
    case ObjectType::SPHERE: {
        return HitSphere(obj.S, r, interval);
    }
    }
    std::unreachable();
}

inline Vec3 SphereNormal(Point3 hit, const Sphere &sphere) noexcept
{
    return (hit - sphere.Center) / sphere.Radius;
}

Vec3 ObjectNormal(Point3 hit, const Object &obj) noexcept
{

    switch (obj.Type)
    {
    case ObjectType::SPHERE: {
        return SphereNormal(hit, obj.S);
    }
    }
    std::unreachable();
}
