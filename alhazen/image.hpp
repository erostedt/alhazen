#pragma once

#include <ostream>

#include "color.hpp"
#include "types.hpp"

struct FloatImage
{
    u32 Width;
    u32 Height;
    Color *Pixels;

    inline Color &operator[](u32 x, u32 y) noexcept
    {
        return Pixels[y * Width + x];
    }

    inline Color &operator[](u32 x, u32 y) const noexcept
    {
        return Pixels[y * Width + x];
    }
};

FloatImage CreateFloatImage(u32 width, u32 height);
FloatImage LoadFloatImage(const char *path);
void FreeFloatImage(FloatImage *image);
void WriteFloatImageToPPM(std::ostream &dst, FloatImage image);
