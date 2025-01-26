#pragma once

#include "types.hpp"

struct Color
{
    f32 Red;
    f32 Green;
    f32 Blue;

    Color &operator+=(const Color &rhs);
};

Color LinearBlend(Color c1, Color c2, f32 a);

Color operator+(Color c1, Color c2);
Color operator/(Color c1, f32 s);

const Color BLACK = {0, 0, 0};
