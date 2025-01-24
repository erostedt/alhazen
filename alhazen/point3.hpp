#pragma once

#include "types.hpp"

struct Vec3;
struct Point3
{
    f32 X;
    f32 Y;
    f32 Z;
};

Vec3 operator-(Point3 p, Point3 q);

Point3 operator+(Point3 p, Vec3 v);
Point3 operator-(Point3 p, Vec3 v);
