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

Vec3 operator-(Vec3 v);

Vec3 operator*(Vec3 v, f32 s);
Vec3 operator*(f32 s, Vec3 v);

Vec3 operator/(Vec3 v, f32 s);

f32 Dot(Vec3 u, Vec3 v);
f32 SquaredLength(Vec3 v);
f32 Length(Vec3 v);
Vec3 Normalized(Vec3 v);
Vec3 Cross(Vec3 u, Vec3 v);

bool NearZero(Vec3 v, f32 tolerance = 1e-8f);

Vec3 RandomUnitVector();
Vec3 RandomVectorOnHemisphere(Vec3 normal);
Vec3 RandomVectorOnUnitDisk();

const Vec3 RIGHT = {1.0f, 0.0f, 0.0f};
const Vec3 UP = {0.0f, 1.0f, 0.0f};
const Vec3 FORWARD = {0.0f, 0.0f, -1.0f};
