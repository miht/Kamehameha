#include "color.h"
#include "mathops.h"

Color::Color()
{
    r = g = b = 0.0;
}

Color::Color(int r, int g, int b)
    :   r(r), g(g), b(b)
{

}

Color::Color(Vector3D rgb_vector)
{
    r = MathOps::clamp(rgb_vector.x, 0, 255);
    g = MathOps::clamp(rgb_vector.y, 0, 255);
    b = MathOps::clamp(rgb_vector.z, 0, 255);
}

QColor Color::asQColor() {
    return QColor(r, g, b);
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

