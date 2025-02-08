#pragma once

#include "point3.hpp"
#include "random.hpp"
#include "types.hpp"
#include "vec3.hpp"

static const u32 POINT_COUNT = 256;
struct PerlinNoise
{
    Vec3 Vectors[POINT_COUNT];
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
        noise.Vectors[i] = RandomUnitVector();
    }

    GeneratePermutation(noise.Perm_X, POINT_COUNT);
    GeneratePermutation(noise.Perm_Y, POINT_COUNT);
    GeneratePermutation(noise.Perm_Z, POINT_COUNT);
    return noise;
}

inline f32 SamplePerlinNoise(const PerlinNoise &noise, const Point3 &sample_point)
{
    i32 x = (i32)std::floor(sample_point.X);
    i32 y = (i32)std::floor(sample_point.Y);
    i32 z = (i32)std::floor(sample_point.Z);

    f32 u = sample_point.X - std::floor(sample_point.X);
    f32 v = sample_point.Y - std::floor(sample_point.Y);
    f32 w = sample_point.Z - std::floor(sample_point.Z);

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

                Vec3 value = noise.Vectors[x_value ^ y_value ^ z_value];
                Vec3 weights = {(u - (f32)dx), (v - (f32)dy), (w - (f32)dz)};
                accum += x_contrib * y_contrib * z_contrib * Dot(value, weights);
            }
        }
    }
    return accum;
}

inline f32 SampleTurbulence(const PerlinNoise &noise, Point3 sample_point, i32 depth)
{
    f32 accum = 0.0f;
    f32 weight = 1.0f;
    for (int i = 0; i < depth; i++)
    {
        accum += weight * SamplePerlinNoise(noise, sample_point);
        weight *= 0.5f;
        sample_point *= 2;
    }

    return std::fabs(accum);
}
