#ifndef LIGHT_H
#define LIGHT_H

#include "vector3d.h"

class Light
{
public:
    Vector3D position;
    Light(Vector3D position);
    Light();
};

#endif // LIGHT_H
