#include "color.hpp"

Color &Color::operator+=(const Color &rhs)
{
    Red += rhs.Red;
    Green += rhs.Green;
    Blue += rhs.Blue;
    return *this;
}

Color operator+(Color c1, Color c2)
{
    return {c1.Red + c2.Red, c1.Green + c2.Green, c1.Blue + c2.Blue};
}

Color operator*(Color c, f32 s)
{
    return {c.Red * s, c.Green * s, c.Blue * s};
}

Color operator*(f32 s, Color c)
{
    return c * s;
}

Color operator/(Color c, f32 s)
{
    return {c.Red / s, c.Green / s, c.Blue / s};
}

Color LinearBlend(Color c1, Color c2, f32 a)
{
    Color blended;
    blended.Red = a * c1.Red + (1.0f - a) * c2.Red;
    blended.Green = a * c1.Green + (1.0f - a) * c2.Green;
    blended.Blue = a * c1.Blue + (1.0f - a) * c2.Blue;
    return blended;
}
