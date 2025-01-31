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
};

FloatImage CreateFloatImage(u32 width, u32 height);
void FreeFloatImage(FloatImage *image);
void WriteFloatImageToPPM(std::ostream &dst, FloatImage image);
