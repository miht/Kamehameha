#include "mathops.h"

#include <QtMath>

MathOps::MathOps()
{

}

float MathOps::clamp(const float &n, const float &lower, const float &upper) {
    return fmax(lower, fmin(n, upper));
}

