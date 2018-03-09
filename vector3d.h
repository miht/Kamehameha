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
    Vector3D normalized();

    static double dot_prod(Vector3D a, Vector3D b);
    static Vector3D cross_prod(Vector3D a, Vector3D b);

    friend Vector3D operator+(const Vector3D & v1, const Vector3D v2);
    friend Vector3D operator-(const Vector3D & v1, const Vector3D v2);
    friend Vector3D operator*(const double & s, const Vector3D v);

    friend std::ostream& operator<<(std::ostream& Str, const Vector3D& v);
    friend QDebug operator<< (QDebug d, const Vector3D &v);
};

#endif // VECTOR_H
