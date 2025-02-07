#include "object.hpp"
#include "box.hpp"

#include <cassert>
#include <cmath>

using namespace ObjectTypes;

static f32 SphereHit(const Object &object, const Ray &r, Interval interval)
{
    const Sphere &sphere = object.Sphere;
    Vec3 oc = sphere.CenterAt(r.Time) - r.Origin;
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

inline Vec3 SphereNormal(const Object &object, const Point3 &hit, f32 time) noexcept
{
    const Sphere &sphere = object.Sphere;
    return (hit - sphere.CenterAt(time)) / sphere.Radius;
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
    obj.Normal = SphereNormal;
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
    obj.Normal = SphereNormal;
    return obj;
}
