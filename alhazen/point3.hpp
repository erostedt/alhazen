#pragma once

#include "types.hpp"
#include "vec3.hpp"

struct Point3
{
    f32 X;
    f32 Y;
    f32 Z;
};

inline Point3 operator+(Point3 p, Vec3 v)
{
    return {p.X + v.X, p.Y + v.Y, p.Z + v.Z};
}

inline Vec3 operator-(Point3 p, Point3 q)
{
    return {p.X - q.X, p.Y - q.Y, p.Z - q.Z};
}

inline Point3 operator-(Point3 p, Vec3 v)
{
    return {p.X - v.X, p.Y - v.Y, p.Z - v.Z};
}

inline Point3 operator*(Point3 p, f32 s)
{
    return {p.X * s, p.Y * s, p.Z * s};
}

const Point3 ORIGIN = {0.0f, 0.0f, 0.0f};
