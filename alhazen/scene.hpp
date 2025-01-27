#pragma once

#include <vector>

#include "material.hpp"
#include "point3.hpp"
#include "types.hpp"

struct Sphere
{
    Point3 Center;
    f32 Radius;
    u32 MaterialIndex;
};

struct Scene
{
    std::vector<Material> Materials;
    std::vector<Sphere> Spheres;
};
