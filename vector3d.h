#ifndef VECTOR_H
#define VECTOR_H


class Vector3D
{
public:
    double x;
    double y;
    double z;
    Vector3D(double x, double y, double z);
    double norm();
    double dot_prod(Vector3D a, Vector3D b);
    Vector3D cross_prod(Vector3D a, Vector3D b);
};

#endif // VECTOR_H
