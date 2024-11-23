#include "Interval.h"

const Interval Interval::empty = Interval(+INFINITY, -INFINITY);
const Interval Interval::universe = Interval(-INFINITY, +INFINITY);

Interval operator+(const Interval& ival, float displacement)
{
    return Interval(ival.min + displacement, ival.max + displacement);
}

Interval operator+(float displacement, const Interval& ival)
{
    return ival + displacement;
}
