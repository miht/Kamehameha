#include "intersection.h"

Intersection::Intersection(Vector3D point, Vector3D normal, QString material)
    :   point(point), normal(normal), material(material)
{
}

Intersection::Intersection() {
    point = Vector3D(0,0,0);
    normal = Vector3D(0,0,0);
    material = "None";
}

bool Intersection::didHit() {
    return hit;
}

