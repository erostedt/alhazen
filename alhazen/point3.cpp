#include "point3.hpp"

#include "vec3.hpp"

Vec3 operator-(Point3 p, Point3 q)
{
    return {p.X - q.X, p.Y - q.Y, p.Z - q.Z};
}

Point3 operator+(Point3 p, Vec3 v)
{
    return {p.X + v.X, p.Y + v.Y, p.Z + v.Z};
}

Point3 operator-(Point3 p, Vec3 v)
{
    return {p.X - v.X, p.Y - v.Y, p.Z - v.Z};
}
