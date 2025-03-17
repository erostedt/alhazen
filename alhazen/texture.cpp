#include "texture.hpp"

#include <algorithm>

#include "color.hpp"
#include "perlin.hpp"

using namespace TextureTypes;

static Color SampleSolidColor(const Texture &texture, UV uv, const Point3 &sample_point) noexcept
{
    (void)uv;
    (void)sample_point;
    return texture.SolidColor.Albedo;
}

static Color SampleCheckerTexture(const Texture &texture, UV uv, const Point3 &sample_point) noexcept
{
    (void)uv;
    CheckerTexture checker_texture = texture.CheckerTexture;
    f32 inv_scale = checker_texture.InverseScale;
    i32 x = (i32)(inv_scale * sample_point.X);
    i32 y = (i32)(inv_scale * sample_point.Y);
    i32 z = (i32)(inv_scale * sample_point.Z);

    bool is_even = (x + y + z) % 2 == 0;

    return is_even ? checker_texture.EvenColor : checker_texture.OddColor;
}

static Color SampleImageTexture(const Texture &texture, UV uv, const Point3 &sample_point) noexcept
{
    (void)sample_point;
    f32 u = std::clamp(uv.U, 0.0f, 1.0f);
    f32 v = std::clamp(uv.V, 0.0f, 1.0f);

    u32 i = (u32)(u * (f32)(texture.Image.Width - 1));
    u32 j = (u32)(v * (f32)(texture.Image.Height - 1));

    return texture.Image[i, j];
}

static Color SampleNoiseTexture(const Texture &texture, UV uv, const Point3 &sample_point) noexcept
{
    (void)uv;
    const PerlinNoise &noise = texture.NoiseTexture.Noise;
    f32 scale = texture.NoiseTexture.Scale;

    return WHITE * 0.5f * (1.0f + std::sin(scale * sample_point.Z + 10.0f * SampleTurbulence(noise, sample_point, 7)));
}

Texture CreateSolidColor(Color albedo)
{
    TextureTypes::SolidColor solid_color = {albedo};
    Texture texture;
    texture.SolidColor = solid_color;
    texture.Sample = SampleSolidColor;
    return texture;
}

Texture CreateCheckerTexture(f32 scale, const Color &even_color, const Color &odd_color)
{
    CheckerTexture checker_texture = {1.0f / scale, even_color, odd_color};
    Texture texture;
    texture.CheckerTexture = checker_texture;
    texture.Sample = SampleCheckerTexture;
    return texture;
}

Texture CreateImageTexture(const FloatImage &image)
{
    Texture texture;
    texture.Image = image;
    texture.Sample = SampleImageTexture;
    return texture;
}

Texture CreateNoiseTexture(const PerlinNoise &noise, f32 scale)
{
    NoiseTexture noise_texture;
    noise_texture.Noise = noise;
    noise_texture.Scale = scale;

    Texture texture;
    texture.NoiseTexture = noise_texture;
    texture.Sample = SampleNoiseTexture;
    return texture;
}
