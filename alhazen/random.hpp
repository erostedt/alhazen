#pragma once

#include <limits>

#include "types.hpp"

static u32 PCGHash(u32 input)
{
    u32 state = input * 747796405u + 2891336453u;
    u32 word = ((state >> ((state >> 28u) + 4u)) ^ state) * 277803737u;
    return (word >> 22u) ^ word;
}

static u32 s_Seed = 0u;

inline u32 UniformU32()
{
    s_Seed = PCGHash(s_Seed);
    return s_Seed;
}

inline f32 UniformF32()
{
    u32 value = UniformU32();
    return (f32)value / (f32)std::numeric_limits<u32>::max();
}

inline f32 UniformF32(f32 min, f32 max)
{
    f32 x = UniformF32();
    return x * (max - min) + min;
}
