#include "ray.hpp"

Point3 Ray::At(f32 t) const
{
    return Origin + t * Direction;
}

bool FrontFacing(const Ray &r, Vec3 outward_normal)
{
    return Dot(r.Direction, outward_normal) < 0.0f;
}
