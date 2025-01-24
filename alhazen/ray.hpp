#pragma once

#include "vec3.hpp"

struct Ray
{
    Vec3 Origin;
    Vec3 Direction;
    Vec3 At(float t) const
    {
        return Origin + t * Direction;
    }
};
