#include "ray.h"

Ray::Ray(Vector3D origin, Vector3D direction)
    :   origin(origin), direction(direction)
{
    Vector3D inv_direction = Vector3D(1/direction.x, 1/direction.y, 1/direction.z);
    sign.x = (inv_direction.x < 0)? -1 : 1;
    sign.y = (inv_direction.y < 0)? -1 : 1;
    sign.z = (inv_direction.z < 0)? -1 : 1;

}

Vector3D Ray::pointOnRay(float dist) {
    return origin + dist * direction;
}

