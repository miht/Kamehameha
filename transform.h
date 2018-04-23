#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "geometry.h"

class Transform
{
public:
    Vector3D position;
    Vector3D rotation;
    Matrix4x4 world;

    Transform();

    Vector3D forward();
    Vector3D up();
    Vector3D right();
};

#endif // TRANSFORM_H
