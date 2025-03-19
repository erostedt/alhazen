#pragma once

#include "box.hpp"
#include "hit_payload.hpp"
#include "interval.hpp"
#include "point3.hpp"
#include "quaternion.hpp"
#include "ray.hpp"
#include "types.hpp"
#include <vector>

struct Object;
typedef bool (*HitFunction)(const Object &obj, const Ray &r, Interval interval, HitPayload &payload);

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
    inline Point3 PositionAt(f32 time) const noexcept
    {
        return Lerp(StartCenter, EndCenter, time);
    }
};

struct Quad
{
    Point3 Anchor;
    Vec3 U;
    Vec3 V;

    Vec3 Normal;
    f32 D;
};

struct ConstantMedium
{
    Object *Boundary;
    f32 NegativeInverseDensity;
};

}; // namespace ObjectTypes

struct Object
{
    HitFunction Hit;

    Box BoundingBox;
    u32 MaterialIndex;
    union {
        ObjectTypes::Sphere Sphere;
        ObjectTypes::Quad Quad;
        ObjectTypes::ConstantMedium ConstantMedium;
    };
};

Object CreateStationarySpere(Point3 center, f32 radius, u32 material_index);
Object CreateMovingSphere(Point3 start_center, Point3 end_center, f32 radius, u32 material_index);
Object CreateQuad(Point3 anchor, Vec3 u, Vec3 v, u32 material_index);
Object CreateConstantMedium(Object *object, f32 density, u32 material_index);
std::vector<Object> CreateBox(const Point3 &p, const Point3 &q, const Quaternion &rotation, u32 material_index);
