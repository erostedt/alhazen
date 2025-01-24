#include "interval.hpp"

bool Interval::Contains(f32 x) const
{
    return (x >= LowerBound) && (x <= UpperBound);
}
