
#include "texture.hpp"

using namespace TextureTypes;

static Color SampleSolidColor(const Texture &texture, f32 u, f32 v, const Point3 &p)
{
    (void)u;
    (void)v;
    (void)p;
    return texture.SolidColor.Albedo;
}

static Color SampleCheckerTexture(const Texture &texture, f32 u, f32 v, const Point3 &p)
{
    (void)u;
    (void)v;
    CheckerTexture checker_texture = texture.CheckerTexture;
    f32 inv_scale = checker_texture.InverseScale;
    i32 x = (i32)(inv_scale * p.X);
    i32 y = (i32)(inv_scale * p.Y);
    i32 z = (i32)(inv_scale * p.Z);

    bool is_even = (x + y + z) % 2 == 0;

    return is_even ? checker_texture.EvenColor.Albedo : checker_texture.OddColor.Albedo;
}

Texture CreateSolidColor(Color albedo)
{
    TextureTypes::SolidColor solid_color = {albedo};
    Texture texture;
    texture.SolidColor = solid_color;
    texture.Sample = SampleSolidColor;
    return texture;
}

Texture CreateCheckerTexture(f32 scale, const SolidColor &even_color, const SolidColor &odd_color)
{
    CheckerTexture checker_texture = {1.0f / scale, even_color, odd_color};
    Texture texture;
    texture.CheckerTexture = checker_texture;
    texture.Sample = SampleCheckerTexture;
    return texture;
}
