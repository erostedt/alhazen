#pragma once

#include <cmath>

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
};

inline Interval Expand(Interval i1, Interval i2) noexcept
{
    Interval expanded;
    expanded.LowerBound = std::fmin(i1.LowerBound, i2.LowerBound);
    expanded.UpperBound = std::fmax(i1.UpperBound, i2.UpperBound);
    return expanded;
}
