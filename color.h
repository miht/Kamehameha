#ifndef COLOR_H
#define COLOR_H

#include <QColor>
#include "vector3d.h"

class Color
{
public:
    int r;
    int g;
    int b;

    Color();
    Color(int r, int g, int b); //range 0-255
    Color(Vector3D rgb_vector);

    void lighten(float value);

    Vector3D asVector3D();
    QColor asQColor();
};

#endif // COLOR_H
