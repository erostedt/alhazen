#pragma once

#include "point3.hpp"
#include "random.hpp"
#include "types.hpp"

static const u32 POINT_COUNT = 256;
struct PerlinNoise
{
    f32 Values[POINT_COUNT];
    i32 Perm_X[POINT_COUNT];
    i32 Perm_Y[POINT_COUNT];
    i32 Perm_Z[POINT_COUNT];
};

inline u32 RandomIndex(u32 max_index) noexcept
{
    return UniformU32() % max_index;
}

inline void GeneratePermutation(i32 *buffer, u32 size)
{
    for (u32 i = 0; i < size; ++i)
    {
        buffer[i] = (i32)i;
    }

    for (u32 i = size - 1; i > 0; --i)
    {
        u32 target = RandomIndex(i);
        i32 tmp = buffer[i];
        buffer[i] = buffer[target];
        buffer[target] = tmp;
    }
}

inline PerlinNoise CreatePerlinNoise() noexcept
{
    PerlinNoise noise{};
    for (u32 i = 0; i < POINT_COUNT; ++i)
    {
        noise.Values[i] = UniformF32();
    }

    GeneratePermutation(noise.Perm_X, POINT_COUNT);
    GeneratePermutation(noise.Perm_Y, POINT_COUNT);
    GeneratePermutation(noise.Perm_Z, POINT_COUNT);
    return noise;
}

inline f32 SamplePerlinNoise(const PerlinNoise &noise, const Point3 &point)
{
    i32 x = (i32)std::floor(point.X);
    i32 y = (i32)std::floor(point.Y);
    i32 z = (i32)std::floor(point.Z);

    f32 u = point.X - std::floor(point.X);
    f32 v = point.Y - std::floor(point.Y);
    f32 w = point.Z - std::floor(point.Z);

    u = u * u * (3.0f - 2.0f * u);
    v = v * v * (3.0f - 2.0f * v);
    w = w * w * (3.0f - 2.0f * w);

    f32 accum = 0.0f;
    for (i32 dx = 0; dx < 2; ++dx)
    {
        f32 x_contrib = (f32)dx * u + (1.0f - (f32)dx) * (1.0f - u);
        i32 x_value = noise.Perm_X[(x + dx) & 255];
        for (i32 dy = 0; dy < 2; ++dy)
        {
            f32 y_contrib = (f32)dy * v + (1.0f - (f32)dy) * (1.0f - v);
            i32 y_value = noise.Perm_Y[(y + dy) & 255];
            for (i32 dz = 0; dz < 2; ++dz)
            {
                f32 z_contrib = (f32)dz * w + (1.0f - (f32)dz) * (1.0f - w);
                i32 z_value = noise.Perm_Z[(z + dz) & 255];

                f32 value = noise.Values[x_value ^ y_value ^ z_value];
                accum += x_contrib * y_contrib * z_contrib * value;
            }
        }
    }
    return accum;
}
