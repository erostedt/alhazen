#include "image.hpp"
#include <iostream>

int main()
{
    FloatImage image = AllocateFloatImage(320, 256);
    for (u32 y = 0; y < image.Height; ++y)
    {
        for (u32 x = 0; x < image.Width; ++x)
        {
            Color &color = image[x, y];
            color.Red = (f32)x / (f32)(image.Width - 1);
            color.Green = (f32)y / (f32)(image.Height - 1);
        }
    }
    WriteFloatImageToPPM(std::cout, image);
}
