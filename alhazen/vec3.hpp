#pragma once

#include <cmath>

#include "types.hpp"

struct Vec3
{
    f32 X;
    f32 Y;
    f32 Z;
};

inline Vec3 operator+(Vec3 u, Vec3 v) noexcept
{
    return {u.X + v.X, u.Y + v.Y, u.Z + v.Z};
}

inline Vec3 operator+(Vec3 v, f32 s) noexcept
{
    return {v.X + s, v.Y + s, v.Z + s};
}

inline Vec3 operator+(f32 s, Vec3 v) noexcept
{
    return v + s;
}

inline Vec3 operator-(Vec3 u, Vec3 v) noexcept
{
    return {u.X - v.X, u.Y - v.Y, u.Z - v.Z};
}

inline Vec3 operator-(Vec3 v, f32 s) noexcept
{
    return {v.X - s, v.Y - s, v.Z - s};
}

inline Vec3 operator-(f32 s, Vec3 v) noexcept
{
    return {s - v.X, s - v.Y, s - v.Z};
}

inline Vec3 operator-(Vec3 v) noexcept
{
    return {-v.X, -v.Y, -v.Z};
}

inline Vec3 operator*(Vec3 v, f32 s) noexcept
{
    return {v.X * s, v.Y * s, v.Z * s};
}

inline Vec3 operator*(f32 s, Vec3 v) noexcept
{
    return v * s;
}

inline Vec3 operator/(Vec3 v, f32 s) noexcept
{
    return {v.X / s, v.Y / s, v.Z / s};
}

inline f32 Dot(Vec3 u, Vec3 v) noexcept
{
    return u.X * v.X + u.Y * v.Y + u.Z * v.Z;
}

inline f32 SquaredLength(Vec3 v) noexcept
{
    return Dot(v, v);
}

inline f32 Length(Vec3 v) noexcept
{
    return std::sqrt(SquaredLength(v));
}

inline Vec3 Normalized(Vec3 v) noexcept
{
    return v / Length(v);
}

inline Vec3 Cross(Vec3 u, Vec3 v) noexcept
{
    Vec3 c;
    c.X = u.Y * v.Z - u.Z * v.Y;
    c.Y = u.Z * v.X - u.X * v.Z;
    c.Z = u.X * v.Y - u.Y * v.X;
    return c;
}

inline bool NearZero(Vec3 v, f32 tolerance = 1e-8f) noexcept
{
    return SquaredLength(v) < tolerance;
}

Vec3 RandomUnitVector() noexcept;
Vec3 RandomVectorOnHemisphere(Vec3 normal) noexcept;
Vec3 RandomVectorOnUnitDisk() noexcept;

const Vec3 ZERO = {0.0f, 0.0f, 0.0f};
const Vec3 RIGHT = {1.0f, 0.0f, 0.0f};
const Vec3 UP = {0.0f, 1.0f, 0.0f};
const Vec3 FORWARD = {0.0f, 0.0f, -1.0f};
