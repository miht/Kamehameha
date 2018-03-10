#include "ray.h"

Ray::Ray(Vector3D origin, Vector3D direction)
    :   origin(origin), direction(direction)
{

}

Vector3D Ray::pointOnRay(float dist) {
    return origin + dist * direction;
}

