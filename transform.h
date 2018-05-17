#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "geometry.h"

class Transform
{
public:
    Vector3D position;
    Vector3D rotation;

    Vector3D forward, up, right, target;

    Matrix4x4 world;

    Transform();

    void lookAt(Vector3D newTar);
};

#endif // TRANSFORM_H
