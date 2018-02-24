#ifndef MODEL3D_H
#define MODEL3D_H
#include <QMap>
#include <vector>

#include "triangle.h"
#include "material.h"

class Model3D
{
public:
    std::vector<Triangle> triangles;
    bool smooth = false;
    QMap<QString, Material> materials;

    Model3D();
};

#endif // MODEL3D_H
