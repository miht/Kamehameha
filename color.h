#ifndef COLOR_H
#define COLOR_H

#include <QColor>
#include "geometry.h"

class Color
{
public:
    float r;
    float g;
    float b;

    Color();
    Color(float r, float g, float b); //range 0-1
    Color(Vector3D rgb_vector);

    void lighten(float value);

    Vector3D asVector3D();
    QColor asQColor();
};

#endif // COLOR_H
