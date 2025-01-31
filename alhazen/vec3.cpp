#include "vec3.hpp"

#include <cmath>

#include "random.hpp"

Vec3 RandomUnitVector() noexcept
{
    Vec3 v;
    while (true)
    {
        v.X = UniformF32(-1.0f, 1.0f);
        v.Y = UniformF32(-1.0f, 1.0f);
        v.Z = UniformF32(-1.0f, 1.0f);

        f32 squared_length = SquaredLength(v);
        if (1e-30f < squared_length && squared_length <= 1.0f)
            return v / std::sqrt(squared_length);
    }
}

Vec3 RandomVectorOnHemisphere(Vec3 normal) noexcept
{
    Vec3 on_unit_sphere = RandomUnitVector();
    if (Dot(on_unit_sphere, normal) > 0.0f)
        return on_unit_sphere;
    else
        return -on_unit_sphere;
}

Vec3 RandomVectorOnUnitDisk() noexcept
{
    Vec3 v;
    v.Z = 0.0f;
    while (true)
    {
        v.X = UniformF32(-1.0f, 1.0f);
        v.Y = UniformF32(-1.0f, 1.0f);
        if (SquaredLength(v) < 1.0f)
            return v;
    }
}
