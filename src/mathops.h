#ifndef MATHOPS_H
#define MATHOPS_H

#include <math.h>

class MathOps
{
public:
    MathOps();

    static float clamp(const float &n, const float &lower, const float &upper);
};

#endif // MATHOPS_H
