#include "object.hpp"

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
    return obj;
}

f32 HitSphere(const Sphere &sphere, const Ray &r, Interval interval)
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

f32 HitObject(const Object &obj, const Ray &r, Interval interval)
{
    switch (obj.Type)
    {
    case ObjectType::SPHERE: {
        return HitSphere(obj.S, r, interval);
    }
    }
    std::unreachable();
}

Vec3 ObjectNormal(Point3 hit, const Object &obj)
{

    switch (obj.Type)
    {
    case ObjectType::SPHERE: {
        return SphereNormal(hit, obj.S);
    }
    }
    std::unreachable();
}
