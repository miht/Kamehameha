#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <QDebug>
#include <QString>
#include <iostream>

#include "face.h"
#include "vertex3d.h"
#include "ray.h"
#include "material.h"
#include "intersection.h"

class Triangle : public Face
{
public:
    Triangle(Vertex3D v1, Vertex3D v2, Vertex3D v3, QString material);
    bool intersects(Ray ray, float &t0, float &t1, Intersection &intersection, bool smooth) override;

    friend std::ostream & operator<<(std::ostream & Str, const Triangle& t);
    friend QDebug operator<< (QDebug d, const Triangle &t);
};

#endif // Triangle_H
