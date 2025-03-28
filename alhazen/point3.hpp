#pragma once

#include "types.hpp"
#include "vec3.hpp"

struct Point3
{
    f32 X;
    f32 Y;
    f32 Z;
};

inline Point3 operator+(Point3 p, Vec3 v) noexcept
{
    return {p.X + v.X, p.Y + v.Y, p.Z + v.Z};
}

inline Vec3 operator-(Point3 p, Point3 q) noexcept
{
    return {p.X - q.X, p.Y - q.Y, p.Z - q.Z};
}

inline Point3 operator-(Point3 p, Vec3 v) noexcept
{
    return {p.X - v.X, p.Y - v.Y, p.Z - v.Z};
}

inline Point3 operator*(Point3 p, f32 s) noexcept
{
    return {p.X * s, p.Y * s, p.Z * s};
}

inline Point3 &operator*=(Point3 &p, f32 s) noexcept
{
    p.X *= s;
    p.Y *= s;
    p.Z *= s;
    return p;
}

inline Vec3 ToVec3(Point3 p) noexcept
{
    return {p.X, p.Y, p.Z};
}

const Point3 ORIGIN = {0.0f, 0.0f, 0.0f};
