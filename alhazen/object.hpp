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

struct Object;
typedef Vec3 (*NormalFunction)(const Object &obj, const Point3 &hit);
typedef f32 (*HitFunction)(const Object &obj, const Ray &r, Interval interval);

struct Object
{
    HitFunction Hit;
    NormalFunction Normal;

    Box BoundingBox;
    u32 MaterialIndex;
    union {
        Sphere S;
    };
};

Object CreateSphere(Point3 center, f32 radius, u32 material_index);
