#ifndef MATERIAL_H
#define MATERIAL_H

#include "geometry.h"
#include "illuminationmodel.h"

class Material
{
public:
    enum Type {
        Lambertian,
        Phong,
        Blinn_Phong
    };

    Type type = Lambertian;

    Vector3D ambient, diffuse, specular, emissive, reflective;
    float spec_exp, dissolved, optical_density, reflectiveFactor;
    IlluminationModel illModel;

    Material();
    friend QDebug operator<< (QDebug d, const Material &m);
};

#endif // MATERIAL_H
