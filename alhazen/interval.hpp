#pragma once

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
};
