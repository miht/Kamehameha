#include "vector3d.h"

Vector3D::Vector3D(float x, float y, float z)
    : x(x), y(y), z(z)
{

}

Vector3D::Vector3D()
{
    x = 0;
    y = 0;
    z = 0;
}

float Vector3D::norm()
{
    return qSqrt(x * x + y * y + z * z);
}

Vector3D Vector3D::normalized() {
    return Vector3D(x / norm(), y / norm(), z / norm());
}

float Vector3D::dot_prod(Vector3D a, Vector3D b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vector3D Vector3D::cross_prod(Vector3D a, Vector3D b)
{
    float x = a.y * b.z - b.y * a.z;
    float y = -(a.x * b.z - b.x * a.z);
    float z = a.x * b.y - b.x * a.y;
    return Vector3D(x, y, z);
}

Vector3D operator+(const Vector3D & v1, const Vector3D v2) {
    return Vector3D(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

Vector3D operator-(const Vector3D & v1, const Vector3D v2) {
    return v1 + -1*v2;
}
Vector3D operator*(const float & s, const Vector3D v) {
    return Vector3D(s * v.x, s * v.y, s * v.z);
}

std::ostream & operator<<(std::ostream & Str, const Vector3D& v) {
    return Str << "(" << v.x << ", " << v.y << ", " << v.z << ")";
}

QDebug operator<< (QDebug d, const Vector3D &v) {
    return d << "(" << v.x << ", " << v.y << ", " << v.z << ")";
}
