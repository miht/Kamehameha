#include "light.h"

Light::Light(Vector3D position, float intensity, Color color)
    :   position(position), intensity(intensity), color(color)
{
}

Light::Light()
{
    position = Vector3D(0,0,0);
    intensity = 1.0;
    color = Color(1,1,1);
}
