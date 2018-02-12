#include "vector3d.h"
#include <QtMath>

Vector3D::Vector3D(double x, double y, double z)
    : x(x), y(y), z(z)
{

}

double Vector3D::norm()
{
    return qSqrt(x * x + y * y + z * z);
}

double Vector3D::dot_prod(Vector3D a, Vector3D b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vector3D Vector3D::cross_prod(Vector3D a, Vector3D b)
{
    double x = a.y * b.z - b.y * a.z;
    double y = -(a.x * b.z - b.x * a.z);
    double z = a.x * b.y - b.x * a.y;
    return Vector3D(x, y, z);
}
