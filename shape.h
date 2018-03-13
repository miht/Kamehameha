#ifndef SHAPE_H
#define SHAPE_H

#include <QString>
#include <vector>

#include "ray.h"
#include "vertex3d.h"
#include "intersection.h"

class Shape
{
public:
    enum Geometry {triangle, quadrilateral, sphere, polygon};
    Geometry type;
    Shape(QString material);

    virtual bool intersects(Ray ray, float &dist, Intersection &intersection);
    virtual std::vector<Vector3D> getPoints() const;

protected:
    std::vector<Vertex3D> vertices;
    QString material;
};

#endif // SHAPE_H
