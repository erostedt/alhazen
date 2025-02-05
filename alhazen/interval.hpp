#pragma once

#include <cmath>
#include <limits>

#include "types.hpp"

struct Interval
{
    f32 LowerBound;
    f32 UpperBound;

    inline bool Contains(f32 x) const
    {
        return (LowerBound <= x) && (x <= UpperBound);
    }

    inline bool Surrounds(f32 x) const
    {
        return (LowerBound < x) && (x < UpperBound);
    }

    inline bool IsEmpty() const noexcept
    {
        return LowerBound > UpperBound;
    }

    inline f32 Width() const noexcept
    {
        return std::fmax(UpperBound - LowerBound, 0.0f);
    }

    inline f32 Midpoint() const noexcept
    {
        return 0.5f * (UpperBound + LowerBound); // Check if valid?
    }

    inline static Interval Empty() noexcept
    {
        Interval interval;
        interval.LowerBound = +std::numeric_limits<f32>::infinity();
        interval.UpperBound = -std::numeric_limits<f32>::infinity();
        return interval;
    }
};

inline Interval Expand(Interval i1, Interval i2) noexcept
{
    Interval expanded;
    expanded.LowerBound = std::min(i1.LowerBound, i2.LowerBound);
    expanded.UpperBound = std::max(i1.UpperBound, i2.UpperBound);
    return expanded;
}

inline Interval Overlap(Interval i1, Interval i2)
{
    Interval overlap;
    overlap.LowerBound = std::fmax(i1.LowerBound, i2.LowerBound);
    overlap.UpperBound = std::fmin(i1.UpperBound, i2.UpperBound);
    return overlap;
}

inline Interval CreateInterval(f32 lower_bound, f32 upper_bound)
{
    Interval interval;
    interval.LowerBound = std::fmin(lower_bound, upper_bound);
    interval.UpperBound = std::fmax(lower_bound, upper_bound);
    return interval;
}
