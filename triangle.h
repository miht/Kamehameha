#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "vertex3d.h"
#include <QDebug>
#include <QString>
#include <iostream>

#include "ray.h"
#include "material.h"
#include "intersection.h"

class Triangle
{
public:
    Vertex3D v1, v2, v3;
    QString material;

    Triangle(Vertex3D v1, Vertex3D v2, Vertex3D v3, QString material);
    bool intersects(Ray ray, float &dist, Intersection &intersection);

    friend std::ostream & operator<<(std::ostream & Str, const Triangle& t);
    friend QDebug operator<< (QDebug d, const Triangle &t);
};

#endif // Triangle_H
