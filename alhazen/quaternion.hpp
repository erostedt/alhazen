#pragma once

#include <cmath>
#include <numbers>

#include "point3.hpp"
#include "types.hpp"
#include "vec3.hpp"

inline f32 Radians(f32 degrees)
{
    return degrees * std::numbers::pi_v<f32> / 180.0f;
}

struct Quaternion
{
    f32 w, x, y, z;
};

inline Quaternion operator*(const Quaternion &q1, const Quaternion &q2)
{
    f32 w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z;
    f32 x = q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y;
    f32 y = q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x;
    f32 z = q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w;

    return {w, x, y, z};
}

inline Quaternion CreateFromAxisAngle(Vec3 axis, float degrees)
{
    axis = Normalized(axis);
    f32 rad = Radians(degrees);
    f32 half_sin = std::sin(0.5f * rad);
    f32 half_cos = std::cos(0.5f * rad);
    return {half_cos, axis.X * half_sin, axis.Y * half_sin, axis.Z * half_sin};
}

inline Vec3 Rotate(const Quaternion &q, const Vec3 &v)
{
    Quaternion qv = {0, v.X, v.Y, v.Z};
    Quaternion q_conjugate = {q.w, -q.x, -q.y, -q.z};
    Quaternion rotated = q * qv * q_conjugate;
    return {rotated.x, rotated.y, rotated.z};
}

inline Point3 RotateAround(const Quaternion &q, const Point3 &point, const Point3 &pivot)
{
    return pivot + Rotate(q, point - pivot);
}

const Quaternion IDENTITY = {1.0f, 0.0f, 0.0f, 0.0f};
