#include "ray.hpp"

Point3 Ray::At(f32 t) const
{
    return Origin + t * Direction;
}
