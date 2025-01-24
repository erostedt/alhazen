#pragma once

#include "types.hpp"

struct Vec3
{
    f32 X;
    f32 Y;
    f32 Z;
};

Vec3 operator+(Vec3 u, Vec3 v);
Vec3 operator+(Vec3 v, f32 s);
Vec3 operator+(f32 s, Vec3 v);

Vec3 operator-(Vec3 u, Vec3 v);
Vec3 operator-(Vec3 v, f32 s);
Vec3 operator-(f32 s, Vec3 v);

Vec3 operator*(Vec3 v, f32 s);
Vec3 operator*(f32 s, Vec3 v);

Vec3 operator/(Vec3 v, f32 s);

f32 Dot(Vec3 u, Vec3 v);
f32 SquaredLength(Vec3 v);
f32 Length(Vec3 v);
Vec3 Normalized(Vec3 v);
