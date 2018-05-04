#ifndef INTERSECTION_H
#define INTERSECTION_H
#include <QString>

#include "geometry.h"

class Intersection
{
public:
    Vector3D point, normal;
    const char* material;
    bool hit = false;

    Intersection();
    Intersection(Vector3D point, Vector3D normal, const char* material);

    bool didHit();
};

#endif // INTERSECTION_H
