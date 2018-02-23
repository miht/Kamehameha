#ifndef VERTEX3D_H
#define VERTEX3D_H

#include "vector3d.h"
#include <QDebug>
#include <iostream>

class Vertex3D
{
public:
    Vector3D position, normal;
    Vertex3D(Vector3D position, Vector3D normal);

    friend std::ostream& operator<<(std::ostream& Str, const Vertex3D& v);
    friend QDebug operator<< (QDebug d, const Vertex3D &v);
};

#endif // VERTEX3D_H
