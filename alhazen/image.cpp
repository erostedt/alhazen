#include "image.hpp"

#include <algorithm>

#include "color.hpp"

FloatImage CreateFloatImage(u32 width, u32 height)
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
    const f32 scale = 255.999f;
    for (u32 y = 0; y < image.Height; ++y)
    {
        for (u32 x = 0; x < image.Width; ++x)
        {
            const Color &rgb = image[x, y];
            Color gamma = LinearToGamma(rgb);

            i32 r = i32(scale * std::clamp(gamma.Red, 0.0f, 1.0f));
            i32 g = i32(scale * std::clamp(gamma.Green, 0.0f, 1.0f));
            i32 b = i32(scale * std::clamp(gamma.Blue, 0.0f, 1.0f));

            dst << r << ' ' << g << ' ' << b << '\n';
        }
    }
}
