#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <QDebug>
#include <QString>
#include <iostream>

#include "shape.h"
#include "vertex3d.h"
#include "ray.h"
#include "material.h"
#include "intersection.h"

class Triangle : public Shape
{
public:
    Triangle(Vertex3D v1, Vertex3D v2, Vertex3D v3, QString material);
    bool intersects(Ray ray, float &dist, Intersection &intersection) override;

    friend std::ostream & operator<<(std::ostream & Str, const Triangle& t);
    friend QDebug operator<< (QDebug d, const Triangle &t);
};

#endif // Triangle_H
