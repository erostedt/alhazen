#pragma once

#include "interval.hpp"
#include "point3.hpp"
struct Box
{
    Interval X;
    Interval Y;
    Interval Z;
};

Box CreateBox(Point3 p, Point3 q);
Box Expand(Box b1, Box b2);
