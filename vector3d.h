#ifndef VECTOR_H
#define VECTOR_H
#include <QtMath>
#include <QDebug>
#include <iostream>


class Vector3D
{
public:
    double x, y, z;
    Vector3D(double x, double y, double z);
    Vector3D();
    double norm();
    double dot_prod(Vector3D a, Vector3D b);
    Vector3D cross_prod(Vector3D a, Vector3D b);

    friend std::ostream& operator<<(std::ostream& Str, const Vector3D& v);
    friend QDebug operator<< (QDebug d, const Vector3D &v);
};

#endif // VECTOR_H
