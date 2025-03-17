#pragma once

#include <cmath>

#include "random.hpp"
#include "types.hpp"

struct Color
{
    f32 Red;
    f32 Green;
    f32 Blue;

    inline Color &operator+=(const Color &rhs) noexcept
    {
        Red += rhs.Red;
        Green += rhs.Green;
        Blue += rhs.Blue;
        return *this;
    }

    inline Color &operator*=(const Color &rhs) noexcept
    {
        Red *= rhs.Red;
        Green *= rhs.Green;
        Blue *= rhs.Blue;
        return *this;
    }
};

inline Color operator+(Color c1, Color c2) noexcept
{
    return {c1.Red + c2.Red, c1.Green + c2.Green, c1.Blue + c2.Blue};
}

inline Color operator*(Color c1, Color c2) noexcept
{
    return {c1.Red * c2.Red, c1.Green * c2.Green, c1.Blue * c2.Blue};
}

inline Color operator*(Color c, f32 s) noexcept
{
    return {c.Red * s, c.Green * s, c.Blue * s};
}

inline Color operator*(f32 s, Color c) noexcept
{
    return c * s;
}

inline Color operator/(Color c, f32 s) noexcept
{
    return {c.Red / s, c.Green / s, c.Blue / s};
}

inline Color LinearBlend(Color c1, Color c2, f32 a) noexcept
{
    return a * c1 + (1.0f - a) * c2;
}

inline Color LinearToGamma(Color c) noexcept
{
    Color gamma;
    gamma.Red = std::pow(c.Red, 1.0f / 2.2f);
    gamma.Green = std::pow(c.Green, 1.0f / 2.2f);
    gamma.Blue = std::pow(c.Blue, 1.0f / 2.2f);
    return gamma;
}

inline Color RandomColor()
{
    return {UniformF32(), UniformF32(), UniformF32()};
}

inline Color RandomColor(f32 min, f32 max)
{
    return {UniformF32(min, max), UniformF32(min, max), UniformF32(min, max)};
}

inline bool AlmostEquals(Color c1, Color c2, f32 tolerance = 1e-6f)
{
    return std::abs(c1.Red - c2.Red) < tolerance && std::abs(c1.Green - c2.Green) < tolerance &&
           std::abs(c1.Blue - c2.Blue) < tolerance;
}

const Color BLACK = {0.0f, 0.0f, 0.0f};
const Color WHITE = {1.0f, 1.0f, 1.0f};
