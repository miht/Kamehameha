#ifndef VERTEX3D_H
#define VERTEX3D_H

#include "geometry.h"
#include <QDebug>
#include <iostream>

class Vertex3D
{
public:
    Vector3D position, normal;
    Vector2D uv;
    Vertex3D(Vector3D position, Vector3D normal, Vector2D uv);

    friend std::ostream& operator<<(std::ostream& Str, const Vertex3D& v);
    friend QDebug operator<< (QDebug d, const Vertex3D &v);
};

#endif // VERTEX3D_H
