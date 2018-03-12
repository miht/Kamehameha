#ifndef SHAPE_H
#define SHAPE_H

#include "ray.h"
#include "intersection.h"

class Shape
{
public:
    enum Geometry {triangle, quadrilateral, sphere, polygon};
    Geometry type;
    Shape();

    virtual bool intersects(Ray ray, float &dist, Intersection &intersection);
};

#endif // SHAPE_H
