#include "image.hpp"
#include <algorithm>

FloatImage AllocateFloatImage(u32 width, u32 height)
{
    FloatImage image;
    image.Width = width;
    image.Height = height;
    image.Pixels = new Color[width * height]();
    return image;
}

void FreeImage(FloatImage *image)
{
    delete[] image->Pixels;
    image->Pixels = nullptr;
    image->Width = 0;
    image->Height = 0;
}

void WriteFloatImageToPPM(std::ostream &dst, FloatImage image)
{
    dst << "P3\n" << image.Width << ' ' << image.Height << "\n255\n";
    for (u32 y = 0; y < image.Height; ++y)
    {
        for (u32 x = 0; x < image.Width; ++x)
        {
            const Color &rgb = image[x, y];

            i32 r = std::clamp(i32(255.999f * rgb.Red), 0, 255);
            i32 g = std::clamp(i32(255.999f * rgb.Green), 0, 255);
            i32 b = std::clamp(i32(255.999f * rgb.Blue), 0, 255);

            dst << r << ' ' << g << ' ' << b << '\n';
        }
    }
}
