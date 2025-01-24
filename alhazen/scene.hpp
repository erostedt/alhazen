#pragma once

#include <vector>

#include "point3.hpp"
#include "types.hpp"

struct Sphere
{
    Point3 Center;
    f32 Radius;
};

struct Scene
{
    std::vector<Sphere> spheres;
};
