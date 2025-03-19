#include "image.hpp"

#include <algorithm>
#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>

#include "color.hpp"

namespace fs = std::filesystem;

FloatImage CreateFloatImage(u32 width, u32 height)
{
    FloatImage image;
    image.Width = width;
    image.Height = height;
    image.Pixels = new Color[width * height]();
    return image;
}

FloatImage LoadFloatImage(const fs::path &path)
{
    assert(path.extension() == "ppm" && "Only ppm P3 files supported");
    std::ifstream file(path);

    std::string type;
    file >> type;
    assert(type == "P3" && "Only ppm P3 files supported");

    i32 width;
    file >> width;
    assert(width > 0 && "Image has no width");

    i32 height;
    file >> height;
    assert(height > 0 && "Image has no height");

    i32 max_value;
    file >> max_value;
    assert(max_value == 255 && "Expected u8 image");

    FloatImage image = CreateFloatImage((u32)width, (u32)height);
    f32 r, g, b;

    for (i32 y = height - 1; y >= 0; --y)
    {
        for (i32 x = 0; x < width; ++x)
        {
            i32 i = y * width + x;
            file >> r;
            file >> g;
            file >> b;
            image.Pixels[i].Red = r / 255.0f;
            image.Pixels[i].Green = g / 255.0f;
            image.Pixels[i].Blue = b / 255.0f;
        }
    }
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
