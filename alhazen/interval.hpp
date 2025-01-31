#pragma once

#include "types.hpp"
#include <cmath>

struct Interval
{
    f32 LowerBound;
    f32 UpperBound;

    inline bool Contains(f32 x) const noexcept
    {
        return (LowerBound <= x) && (x <= UpperBound);
    }

    inline bool Surrounds(f32 x) const noexcept
    {
        return (LowerBound < x) && (x < UpperBound);
    }

    inline bool Empty() const noexcept
    {
        return UpperBound < LowerBound;
    }
};

inline Interval CreateInterval(f32 lower_bound, f32 upper_bound)
{
    Interval interval;
    interval.LowerBound = std::fmin(lower_bound, upper_bound);
    interval.UpperBound = std::fmax(lower_bound, upper_bound);
    return interval;
}

inline Interval Range(Interval i1, Interval i2) noexcept
{
    return {std::fmin(i1.LowerBound, i2.LowerBound), std::fmax(i1.UpperBound, i2.UpperBound)};
}

inline Interval Overlap(Interval i1, Interval i2)
{
    return {std::fmax(i1.LowerBound, i2.LowerBound), std::fmin(i1.UpperBound, i2.UpperBound)};
}
