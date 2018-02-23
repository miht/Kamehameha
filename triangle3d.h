#ifndef TRIANGLE3D_H
#define TRIANGLE3D_H

#include "vertex3d.h"
#include <QDebug>
#include <QString>
#include <iostream>

#include "material.h"

class Triangle3D
{
public:
    Vertex3D v1, v2, v3;
    QString material;

    Triangle3D(Vertex3D v1, Vertex3D v2, Vertex3D v3, QString material);
    friend std::ostream & operator<<(std::ostream & Str, const Triangle3D& t);
    friend QDebug operator<< (QDebug d, const Triangle3D &t);
};

#endif // TRIANGLE3D_H
