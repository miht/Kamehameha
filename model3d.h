#ifndef MODEL3D_H
#define MODEL3D_H
#include "triangle3d.h"
#include <vector>

class Model3D
{
public:
    std::vector<Triangle3D> triangles;
    Model3D();
};

#endif // MODEL3D_H
