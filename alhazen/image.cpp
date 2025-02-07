#include "image.hpp"

#include <algorithm>
#include <iostream>

#include "color.hpp"

#define STB_IMAGE_IMPLEMENTATION
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall" // Disable warnings
#pragma GCC diagnostic ignored "-Wextra"
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Wconversion"
#include "stb_image.h"
#pragma GCC diagnostic pop

FloatImage CreateFloatImage(u32 width, u32 height)
{
    FloatImage image;
    image.Width = width;
    image.Height = height;
    image.Pixels = new Color[width * height]();
    return image;
}

FloatImage LoadFloatImage(const char *path)
{
    i32 width, height, channels_in_file;
    stbi_set_flip_vertically_on_load(true);
    u8 *data = stbi_load(path, &width, &height, &channels_in_file, STBI_rgb);

    assert(data != nullptr);

    FloatImage image = CreateFloatImage((u32)width, (u32)height);
    for (u32 i = 0; i < image.Width * image.Height; ++i)
    {
        u32 offset = i * 3;
        image.Pixels[i].Red = (f32)data[offset + 0] / 255.0f;
        image.Pixels[i].Green = (f32)data[offset + 1] / 255.0f;
        image.Pixels[i].Blue = (f32)data[offset + 2] / 255.0f;
    }

    stbi_image_free(data);
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
