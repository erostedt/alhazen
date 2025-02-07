#pragma once

#include "box.hpp"
#include "interval.hpp"
#include "point3.hpp"
#include "ray.hpp"
#include "types.hpp"

struct UV
{
    f32 U;
    f32 V;
};

struct Object;
typedef Vec3 (*NormalFunction)(const Object &obj, const Point3 &hit, f32 time);
typedef f32 (*HitFunction)(const Object &obj, const Ray &r, Interval interval);
typedef UV (*UVFunction)(const Object &obj, const Vec3 &normal);

inline Point3 Lerp(Point3 a, Point3 b, f32 t) noexcept
{
    return a + (b - a) * t;
}

namespace ObjectTypes
{

struct Sphere
{
    Point3 StartCenter;
    Point3 EndCenter;
    f32 Radius;
    inline Point3 CenterAt(f32 time) const noexcept
    {
        return Lerp(StartCenter, EndCenter, time);
    }
};

}; // namespace ObjectTypes

struct Object
{
    HitFunction Hit;
    NormalFunction Normal;
    UVFunction UV;

    Box BoundingBox;
    u32 MaterialIndex;
    union {
        ObjectTypes::Sphere Sphere;
    };
};

Object CreateStationarySpere(Point3 center, f32 radius, u32 material_index);
Object CreateMovingSphere(Point3 start_center, Point3 end_center, f32 radius, u32 material_index);
