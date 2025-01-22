#include "image.hpp"
#include <iostream>

FloatImage AllocateFloatImage(u32 width, u32 height)
{
    FloatImage image;
    image.Width = width;
    image.Height = height;
    image.Pixels = new RGB[width * height]();
    return image;
}

void FreeImage(FloatImage *image)
{
    delete[] image->Pixels;
    image->Pixels = nullptr;
    image->Width = 0;
    image->Height = 0;
}

void WriteFloatImageToPPM(FloatImage image)
{
    std::cout << "P3\n" << image.Width << ' ' << image.Height << "\n255\n";

    for (u32 y = 0; y < image.Height; ++y)
    {
        for (u32 x = 0; x < image.Width; ++x)
        {
            const RGB &rgb = image[x, y];

            int ir = int(255.999f * rgb.Red);
            int ig = int(255.999f * rgb.Green);
            int ib = int(255.999f * rgb.Blue);

            std::cout << ir << ' ' << ig << ' ' << ib << '\n';
        }
    }
}
