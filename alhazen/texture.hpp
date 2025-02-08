#pragma once

#include "color.hpp"
#include "image.hpp"
#include "object.hpp"
#include "perlin.hpp"
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

struct NoiseTexture
{
    PerlinNoise Noise;
    f32 Scale;
};

}; // namespace TextureTypes

typedef Color (*SampleTexture)(const Texture &texture, UV uv, const Point3 &sample_point);

struct Texture
{
    union {
        TextureTypes::SolidColor SolidColor;
        TextureTypes::CheckerTexture CheckerTexture;
        FloatImage Image;
        TextureTypes::NoiseTexture NoiseTexture;
    };

    SampleTexture Sample;
};

Texture CreateSolidColor(Color albedo);
Texture CreateCheckerTexture(f32 scale, const Color &even_color, const Color &odd_color);
Texture CreateImageTexture(const FloatImage &image);
Texture CreateNoiseTexture(const PerlinNoise &noise, f32 scale);
