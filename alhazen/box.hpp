#pragma once

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
        empty.YAxis = Interval::Empty();
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

inline Box Expand(const Box &b1, const Box &b2)
{
    Box expanded;
    expanded.XAxis = Expand(b1.XAxis, b2.XAxis);
    expanded.YAxis = Expand(b1.YAxis, b2.YAxis);
    expanded.ZAxis = Expand(b1.ZAxis, b2.ZAxis);
    return expanded;
}

inline bool HitsBox(const Box &box, const Ray &ray, Interval interval) noexcept
{
    f32 hit1 = (box.XAxis.LowerBound - ray.Origin.X) / ray.Direction.X;
    f32 hit2 = (box.XAxis.UpperBound - ray.Origin.X) / ray.Direction.X;
    Interval axis = CreateInterval(hit1, hit2);
    interval = Overlap(axis, interval);
    if (interval.IsEmpty())
    {
        return false;
    }

    hit1 = (box.YAxis.LowerBound - ray.Origin.Y) / ray.Direction.Y;
    hit2 = (box.YAxis.UpperBound - ray.Origin.Y) / ray.Direction.Y;
    axis = CreateInterval(hit1, hit2);
    interval = Overlap(axis, interval);
    if (interval.IsEmpty())
    {
        return false;
    }

    hit1 = (box.ZAxis.LowerBound - ray.Origin.Z) / ray.Direction.Z;
    hit2 = (box.ZAxis.UpperBound - ray.Origin.Z) / ray.Direction.Z;
    axis = CreateInterval(hit1, hit2);
    interval = Overlap(axis, interval);
    return !interval.IsEmpty();
}
