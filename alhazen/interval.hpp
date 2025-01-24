#pragma once

#include "types.hpp"

struct Interval
{
    f32 LowerBound;
    f32 UpperBound;
    bool Contains(f32 x) const;
};

Interval ZeroToInfinity();
