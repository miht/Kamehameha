#ifndef TRIANGLE3D_H
#define TRIANGLE3D_H

#include "vertex3d.h"

class Triangle3D
{
public:

    Vertex3D v1, v2, v3;

    Triangle3D(Vertex3D v1, Vertex3D v2, Vertex3D v3);
};

#endif // TRIANGLE3D_H
