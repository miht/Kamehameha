#include "color.h"
#include "mathops.h"

/**
 * @brief Color::Color with range 0.0 - 1.0
 */

Color::Color()
{
    r = g = b = 0.0;
}

Color::Color(float r, float g, float b)
    :   r(r), g(g), b(b)
{

}

Color::Color(Vector3D rgb_vector)
{
    r = MathOps::clamp(rgb_vector.x, 0.0, 1.0);
    g = MathOps::clamp(rgb_vector.y, 0.0, 1.0);
    b = MathOps::clamp(rgb_vector.z, 0.0, 1.0);
}

QColor Color::asQColor() {
    return QColor(255*r, 255*g, 255*b);
}

Vector3D Color::asVector3D() {
    return Vector3D(r, g, b);
}

void Color::lighten(float value) {
    float lVal = value;
    if(lVal <= 0.0) lVal = 0.0;
    if(lVal >= 1.0) lVal = 1.0;
    r *= lVal;
    g *= lVal;
    b *= lVal;
}

