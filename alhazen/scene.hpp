#pragma once

#include <vector>

#include "bvh.hpp"
#include "material.hpp"

struct Scene
{
    std::vector<Material> Materials;
    BVH Bvh;
    Color Background = {0.70f, 0.80f, 1.00f};
};
