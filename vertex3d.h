#ifndef VERTEX3D_H
#define VERTEX3D_H

#include "vector3d.h"

class Vertex3D
{
public:
    Vector3D position, normal;
    Vertex3D(Vector3D position, Vector3D normal);
};

#endif // VERTEX3D_H
