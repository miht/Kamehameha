#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include "ray.h"
#include "intersection.h"

#include "vector3d.h"

class BoundingBox
{
public:
    Vector3D min, max;
    BoundingBox();
    BoundingBox(Vector3D min, Vector3D max);

    int getLongestAxis();

    void expand(BoundingBox bbox);

    bool intersects(Ray ray, float &t0, float &t1, Intersection &intersection);
};

#endif // BOUNDINGBOX_H
