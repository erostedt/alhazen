#pragma once

#include "point3.hpp"
#include "types.hpp"
#include "uv.hpp"
#include "vec3.hpp"

struct HitPayload
{
    Point3 Position;
    Vec3 Normal;
    f32 Distance;
    UV UVCoordinates;
    i32 ObjectIndex;
    bool FrontFacing;
};
