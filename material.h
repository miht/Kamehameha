#ifndef MATERIAL_H
#define MATERIAL_H

#include "vector3d.h"
#include "illuminationmodel.h"

class Material
{
public:
    Vector3D ambient, diffuse, specular, emissive;
    float spec_exp, dissolved, optical_density;
    IlluminationModel illModel;

    Material();
    friend QDebug operator<< (QDebug d, const Material &m);
};

#endif // MATERIAL_H
