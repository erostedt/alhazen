#pragma once

#include "point3.hpp"
#include "types.hpp"
#include "vec3.hpp"

struct HitPayload
{
    Point3 Position;
    Vec3 Normal;
    f32 Distance;
    f32 U;
    f32 V;
    i32 ObjectIndex;
    bool FrontFacing;
};
