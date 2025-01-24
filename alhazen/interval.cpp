#include "interval.hpp"

#include <limits>

bool Interval::Contains(f32 x) const
{
    return (x >= LowerBound) && (x <= UpperBound);
}

Interval ZeroToInfinity()
{
    return {0.0f, std::numeric_limits<f32>::infinity()};
}
