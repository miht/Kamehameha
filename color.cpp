#include "color.h"

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
    if(rgb_vector.x > 255) {
        r = 255;
    }
    else {
        r = rgb_vector.x;
    }
    if(rgb_vector.y > 255) {
        g = 255;
    }
    else {
        g = rgb_vector.y;
    }
    if(rgb_vector.z > 255) {
        b = 255;
    }
    else {
        b = rgb_vector.z;
    }
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

