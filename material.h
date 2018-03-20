#ifndef MATERIAL_H
#define MATERIAL_H

#include "vector3d.h"

class Material
{
public:
    Vector3D ambient, diffuse, specular;
    float spec_exp = 0.0;

    Material();
    friend QDebug operator<< (QDebug d, const Material &m);
};

#endif // MATERIAL_H
