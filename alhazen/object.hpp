#pragma once

#include "interval.hpp"
#include "point3.hpp"
#include "ray.hpp"
#include "types.hpp"

struct Sphere
{
    Point3 Center;
    f32 Radius;
};

enum class ObjectType
{
    SPHERE
};

struct Object
{
    ObjectType Type;
    u32 MaterialIndex;
    union {
        Sphere S;
    };
};

Object CreateSphere(Point3 center, f32 radius, u32 material_index);

f32 HitObject(const Object &obj, const Ray &r, Interval interval);
f32 HitSphere(const Sphere &sphere, const Ray &r, Interval interval);

Vec3 ObjectNormal(Point3 hit, const Object &obj);
inline Vec3 SphereNormal(Point3 hit, const Sphere &sphere) noexcept
{
    return (hit - sphere.Center) / sphere.Radius;
}
