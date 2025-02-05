#pragma once

#include <vector>

#include "bvh.hpp"
#include "material.hpp"

struct Scene
{
    std::vector<Material> Materials;
    BVH Bvh;
};
