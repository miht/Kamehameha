#ifndef VECTOR_H
#define VECTOR_H
#include <QtMath>
#include <QDebug>
#include <iostream>


class Vector3D
{
public:
    float x, y, z;
    Vector3D(float x, float y, float z);
    Vector3D();
    float norm();
    Vector3D normalized();

    static float dot_prod(Vector3D a, Vector3D b);
    static Vector3D cross_prod(Vector3D a, Vector3D b);
    static Vector3D interpolate(Vector3D a, Vector3D b);

    friend Vector3D operator+(const Vector3D & v1, const Vector3D v2);
    friend Vector3D operator-(const Vector3D & v1, const Vector3D v2);
    friend Vector3D operator*(const float & s, const Vector3D v);

    friend std::ostream& operator<<(std::ostream& Str, const Vector3D& v);
    friend QDebug operator<< (QDebug d, const Vector3D &v);
};

#endif // VECTOR_H
