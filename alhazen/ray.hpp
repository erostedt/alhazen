#pragma once

#include "point3.hpp"
#include "types.hpp"
#include "vec3.hpp"

struct Ray
{
    Point3 Origin;
    Vec3 Direction;
    Point3 At(f32 t) const;
};

inline bool FrontFacing(const Ray &r, Vec3 outward_normal)
{
    return Dot(r.Direction, outward_normal) < 0;
}
