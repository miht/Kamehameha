#ifndef LIGHT_H
#define LIGHT_H

#include "vector3d.h"
#include "color.h"

class Light
{
public:
    Vector3D position;
    float intensity= 1.0;
    Color color;
    Light(Vector3D position, float intensity, Color color);
    Light();
};

#endif // LIGHT_H
