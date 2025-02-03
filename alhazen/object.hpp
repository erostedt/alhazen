#pragma once

#include "box.hpp"
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
    Box BoundingBox;
    union {
        Sphere S;
    };
};

Object CreateSphere(Point3 center, f32 radius, u32 material_index);

f32 HitObject(const Object &obj, const Ray &r, Interval interval) noexcept;
Vec3 ObjectNormal(Point3 hit, const Object &obj) noexcept;
