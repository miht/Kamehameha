#include "light.h"

Light::Light(Vector3D position)
    :   position(position)
{
}

Light::Light()
{
    position = Vector3D(0,0,0);
}
