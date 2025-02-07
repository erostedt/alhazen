#pragma once

#include "color.hpp"
#include "point3.hpp"

struct Texture;
namespace TextureTypes
{

struct SolidColor
{
    Color Albedo;
};

struct CheckerTexture
{
    f32 InverseScale;
    Color EvenColor;
    Color OddColor;
};

}; // namespace TextureTypes

typedef Color (*SampleTexture)(const Texture &texture, f32 u, f32 v, const Point3 &p);

struct Texture
{
    union {
        TextureTypes::SolidColor SolidColor;
        TextureTypes::CheckerTexture CheckerTexture;
    };

    SampleTexture Sample;
};

Texture CreateSolidColor(Color albedo);
Texture CreateCheckerTexture(f32 scale, const Color &even_color, const Color &odd_color);
