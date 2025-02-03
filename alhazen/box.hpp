#pragma once

#include "interval.hpp"
#include "point3.hpp"

struct Box
{
    Interval XAxis;
    Interval YAxis;
    Interval ZAxis;
};

inline Box CreateBox(Point3 p, Point3 q)
{
    Box box;
    box.XAxis = CreateInterval(p.X, q.X);
    box.YAxis = CreateInterval(p.Y, q.Y);
    box.ZAxis = CreateInterval(p.Z, q.Z);
    return box;
}

inline Box Expand(Box b1, Box b2)
{
    Box expanded;
    expanded.XAxis = Expand(b1.XAxis, b2.XAxis);
    expanded.YAxis = Expand(b1.YAxis, b2.YAxis);
    expanded.ZAxis = Expand(b1.ZAxis, b2.ZAxis);
    return expanded;
}
