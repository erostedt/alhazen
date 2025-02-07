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
    SolidColor EvenColor;
    SolidColor OddColor;
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
Texture CreateCheckerTexture(Color albedo);
