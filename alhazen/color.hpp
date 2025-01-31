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
Color LinearToGamma(Color c);

Color RandomColor();
Color RandomColor(f32 min, f32 max);

Color operator+(Color c1, Color c2);

Color operator*(Color c1, Color c2);
Color operator*(Color c, f32 s);
Color operator*(f32 s, Color c);

Color operator/(Color c, f32 s);

const Color BLACK = {0.0f, 0.0f, 0.0f};
const Color WHITE = {1.0f, 1.0f, 1.0f};
