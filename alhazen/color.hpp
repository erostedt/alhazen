#pragma once

#include "types.hpp"

struct Color
{
    f32 Red;
    f32 Green;
    f32 Blue;
};

Color LinearBlend(Color c1, Color c2, f32 a);
