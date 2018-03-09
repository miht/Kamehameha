#ifndef RAY_H
#define RAY_H

#include "vector3d.h"

class Ray
{
public:
    Vector3D origin, direction;
    Ray(Vector3D origin, Vector3D direction);
};

#endif // RAY_H
