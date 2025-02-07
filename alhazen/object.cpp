#include "object.hpp"
#include "box.hpp"

#include <cassert>
#include <cmath>

static f32 SphereHit(const Object &object, const Ray &r, Interval interval)
{
    const Sphere &sphere = object.S;
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

inline Vec3 SphereNormal(const Object &object, const Point3 &hit) noexcept
{
    const Sphere &sphere = object.S;
    return (hit - sphere.Center) / sphere.Radius;
}

Object CreateSphere(Point3 center, f32 radius, u32 material_index)
{
    Sphere sphere{center, radius};
    Object obj;
    obj.S = sphere;
    obj.MaterialIndex = material_index;

    Point3 p = center - radius * ONE;
    Point3 q = center + radius * ONE;
    obj.BoundingBox = CreateBox(p, q);
    obj.Hit = SphereHit;
    obj.Normal = SphereNormal;
    return obj;
}
