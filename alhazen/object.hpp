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

f32 HitObject(Object obj, Ray r, Interval interval);
f32 HitSphere(Sphere sphere, Ray r, Interval interval);

Vec3 ObjectNormal(Point3 hit, Object obj);
Vec3 SphereNormal(Point3 hit, Sphere sphere);
