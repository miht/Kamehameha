#ifndef RAY_H
#define RAY_H

#include "vector3d.h"

class Ray
{
public:
    Vector3D sign, origin, direction;

    Ray();
    Ray(Vector3D origin, Vector3D direction);
    Vector3D pointOnRay(float dist);
};

#endif // RAY_H
