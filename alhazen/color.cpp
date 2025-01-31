#include "color.hpp"

#include <cmath>

#include "random.hpp"

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

Color operator*(Color c1, Color c2)
{
    return {c1.Red * c2.Red, c1.Green * c2.Green, c1.Blue * c2.Blue};
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
    return a * c1 + (1.0f - a) * c2;
}

Color LinearToGamma(Color c)
{
    Color gamma;
    gamma.Red = std::pow(c.Red, 1.0f / 2.2f);
    gamma.Green = std::pow(c.Green, 1.0f / 2.2f);
    gamma.Blue = std::pow(c.Blue, 1.0f / 2.2f);
    return gamma;
}

Color RandomColor()
{
    return {UniformF32(), UniformF32(), UniformF32()};
}

Color RandomColor(f32 min, f32 max)
{
    return {UniformF32(min, max), UniformF32(min, max), UniformF32(min, max)};
}
