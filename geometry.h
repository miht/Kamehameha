#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <QDebug>
#include <iostream>

#include "math.h"

class Vector2D
{
public:
    float x, y;
    Vector2D();
    Vector2D(float x, float y);
};

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
    static Vector3D reflect(Vector3D vec, Vector3D normal);
    static Vector3D interpolate(Vector3D a, Vector3D b);

    static Vector3D zero();
    static Vector3D up();
    static Vector3D right();
    static Vector3D forward();

    Vector3D operator+(const Vector3D v) const;
    Vector3D operator-(const Vector3D v) const;
    Vector3D operator-() const;
    Vector3D operator*(const Vector3D v) const;
    Vector3D operator*(const float s) const;
    Vector3D operator/(const float d) const;

    friend std::ostream& operator<<(std::ostream& Str, const Vector3D& v);
    friend QDebug operator<< (QDebug d, const Vector3D &v);
};

//commutativity
Vector3D operator*(const float s, const Vector3D v);

class Matrix4x4
{
public:
    float elements[16] = {};
    Matrix4x4();

    static Matrix4x4 identity();
    static Matrix4x4 translation(const Vector3D v);
    static Matrix4x4 rotation(const Vector3D v);

    float& operator()(const int i, const int j);

    Matrix4x4 operator*(Matrix4x4 m);
    Vector3D operator*(const Vector3D v);

    friend std::ostream& operator<<(std::ostream& Str, Matrix4x4& v);
    friend QDebug operator<<(QDebug d, Matrix4x4 &m);
};

class Ray
{
public:
    Vector3D sign, origin, direction;

    Ray();
    Ray(Vector3D origin, Vector3D direction);
    Vector3D pointOnRay(float dist);
};

#endif // GEOMETRY_H