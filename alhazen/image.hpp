#pragma once

#include <ostream>

#include "types.hpp"

struct RGB
{
    f32 Red;
    f32 Green;
    f32 Blue;
};

struct FloatImage
{
    u32 Width;
    u32 Height;
    RGB *Pixels;

    RGB &operator[](u32 x, u32 y)
    {
        return Pixels[y * Width + x];
    }
};

FloatImage AllocateFloatImage(u32 width, u32 height);
void FreeFloatImage(FloatImage *image);
void WriteFloatImageToPPM(std::ostream &dst, FloatImage image);
