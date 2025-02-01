#include "box.hpp"
#include "interval.hpp"

Box CreateBox(Point3 p, Point3 q)
{
    f32 xmin = std::fmin(p.X, q.X);
    f32 xmax = std::fmax(p.X, q.X);

    f32 ymin = std::fmin(p.Y, q.Y);
    f32 ymax = std::fmax(p.Y, q.Y);

    f32 zmin = std::fmin(p.Y, q.Y);
    f32 zmax = std::fmax(p.Y, q.Y);

    Interval x = {xmin, xmax};
    Interval y = {ymin, ymax};
    Interval z = {zmin, zmax};
    return {x, y, z};
}

Box Expand(Box b1, Box b2)
{
    Box merged;
    merged.X = Expand(b1.X, b2.X);
    merged.Y = Expand(b1.Y, b2.Y);
    merged.Z = Expand(b1.Z, b2.Z);
    return merged;
}
