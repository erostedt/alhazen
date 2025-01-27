#include "vec3.hpp"

#include <cmath>

#include "random.hpp"

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

Vec3 operator-(Vec3 v)
{
    return {-v.X, -v.Y, -v.Z};
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
    return u.X * v.X + u.Y * v.Y + u.Z * v.Z;
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

Vec3 RandomUnitVector()
{
    while (true)
    {
        Vec3 v;
        v.X = UniformF32(-1.0f, 1.0f);
        v.Y = UniformF32(-1.0f, 1.0f);
        v.Z = UniformF32(-1.0f, 1.0f);

        f32 squared_length = SquaredLength(v);
        if (1e-30f < squared_length && squared_length <= 1.0f)
            return v / std::sqrt(squared_length);
    }
}

Vec3 RandomVectorOnHemisphere(Vec3 normal)
{
    Vec3 on_unit_sphere = RandomUnitVector();
    if (Dot(on_unit_sphere, normal) > 0.0f)
        return on_unit_sphere;
    else
        return -on_unit_sphere;
}
