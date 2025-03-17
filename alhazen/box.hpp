#pragma once

#include <algorithm>

#include "interval.hpp"
#include "point3.hpp"
#include "ray.hpp"

struct Box
{
    Interval XAxis;
    Interval YAxis;
    Interval ZAxis;

    inline static Box Empty()
    {
        Box empty;
        empty.XAxis = Interval::Empty();
        empty.YAxis = Interval::Empty();
        empty.ZAxis = Interval::Empty();
        return empty;
    }

    inline u32 LongestAxis()
    {
        if (XAxis.Width() > YAxis.Width() && XAxis.Width() > ZAxis.Width())
        {
            return 0;
        }

        if (YAxis.Width() > ZAxis.Width())
        {
            return 1;
        }

        return 2;
    }
};

inline Box CreateBox(Point3 p, Point3 q)
{
    Box box;
    box.XAxis = CreateInterval(p.X, q.X);
    box.YAxis = CreateInterval(p.Y, q.Y);
    box.ZAxis = CreateInterval(p.Z, q.Z);
    return box;
}

inline Box Expand(const Box &b1, const Box &b2) noexcept
{
    Box expanded;
    expanded.XAxis = Expand(b1.XAxis, b2.XAxis);
    expanded.YAxis = Expand(b1.YAxis, b2.YAxis);
    expanded.ZAxis = Expand(b1.ZAxis, b2.ZAxis);
    return expanded;
}

inline Box ExpandToAtleast(const Box &b, f32 minimum) noexcept
{
    Box expanded = b;
    if (b.XAxis.Width() < minimum)
    {
        f32 delta = minimum - b.XAxis.Width();
        expanded.XAxis = Expand(b.XAxis, delta);
    }
    if (b.YAxis.Width() < minimum)
    {
        f32 delta = minimum - b.YAxis.Width();
        expanded.YAxis = Expand(b.YAxis, delta);
    }
    if (b.ZAxis.Width() < minimum)
    {
        f32 delta = minimum - b.ZAxis.Width();
        expanded.ZAxis = Expand(b.ZAxis, delta);
    }
    return expanded;
}

inline bool HitsBox(const Box &box, const Ray &ray, Interval interval) noexcept
{
    f32 bound_min, bound_max;
    f32 t_min, t_max;

    f32 xscl = 1.0f / ray.Direction.X;
    bound_min = (box.XAxis.LowerBound - ray.Origin.X) * xscl;
    bound_max = (box.XAxis.UpperBound - ray.Origin.X) * xscl;

    t_min = std::min(bound_min, bound_max);
    t_max = std::max(bound_min, bound_max);

    interval.LowerBound = std::max(t_min, interval.LowerBound);
    interval.UpperBound = std::min(t_max, interval.UpperBound);

    if (interval.LowerBound > interval.UpperBound)
    {
        return false;
    }

    f32 yscl = 1.0f / ray.Direction.Y;
    bound_min = (box.YAxis.LowerBound - ray.Origin.Y) * yscl;
    bound_max = (box.YAxis.UpperBound - ray.Origin.Y) * yscl;

    t_min = std::min(bound_min, bound_max);
    t_max = std::max(bound_min, bound_max);

    interval.LowerBound = std::max(t_min, interval.LowerBound);
    interval.UpperBound = std::min(t_max, interval.UpperBound);

    if (interval.LowerBound > interval.UpperBound)
    {
        return false;
    }

    f32 zscl = 1.0f / ray.Direction.Z;
    bound_min = (box.ZAxis.LowerBound - ray.Origin.Z) * zscl;
    bound_max = (box.ZAxis.UpperBound - ray.Origin.Z) * zscl;

    t_min = std::min(bound_min, bound_max);
    t_max = std::max(bound_min, bound_max);

    interval.LowerBound = std::max(t_min, interval.LowerBound);
    interval.UpperBound = std::min(t_max, interval.UpperBound);

    return interval.LowerBound <= interval.UpperBound;
}
