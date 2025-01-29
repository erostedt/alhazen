#pragma once

#include <vector>

#include "material.hpp"
#include "object.hpp"

struct Scene
{
    std::vector<Material> Materials;
    std::vector<Object> Objects;
};
