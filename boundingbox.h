#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include "ray.h"

#include "vector3d.h"

class BoundingBox
{
public:
    Vector3D min, max;
    BoundingBox(Vector3D min, Vector3D max);

    bool intersects(Ray ray, float &t0, float &t1);
};

#endif // BOUNDINGBOX_H
