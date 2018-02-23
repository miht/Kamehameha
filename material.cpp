#include "material.h"

Material::Material()
{

}

QDebug operator<< (QDebug d, const Material &m) {
    return d << "a:" << m.ambient << "\nd:" << m.diffuse << "\ns:" << m.specular <<"\nse:" << m.spec_exp;
}
