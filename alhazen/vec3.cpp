#include "vec3.hpp"

#include <cmath>

Vec3 operator+(Vec3 u, Vec3 v)
{
    return {u.X + v.X, u.Y + v.Y, u.Z + v.Z};
}

Vec3 operator+(Vec3 v, f32 s)
{
    return {v.X + s, v.Y + s, v.Z + s};
}

Vec3 operator+(f32 s, Vec3 v)
{
    return v + s;
}

Vec3 operator-(Vec3 u, Vec3 v)
{
    return {u.X - v.X, u.Y - v.Y, u.Z - v.Z};
}

Vec3 operator-(Vec3 v, f32 s)
{
    return {v.X - s, v.Y - s, v.Z - s};
}

Vec3 operator-(f32 s, Vec3 v)
{
    return {s - v.X, s - v.Y, s - v.Z};
}

Vec3 operator*(Vec3 v, f32 s)
{
    return {v.X * s, v.Y * s, v.Z * s};
}

Vec3 operator*(f32 s, Vec3 v)
{
    return v * s;
}

Vec3 operator/(Vec3 v, f32 s)
{
    return {v.X / s, v.Y / s, v.Z / s};
}

f32 Dot(Vec3 u, Vec3 v)
{
    return u.X * v.X + u.Y * v.Y + u.Z * u.Z;
}

f32 SquaredLength(Vec3 v)
{
    return Dot(v, v);
}

f32 Length(Vec3 v)
{
    return std::sqrt(SquaredLength(v));
}

Vec3 Normalized(Vec3 v)
{
    return v / Length(v);
}
