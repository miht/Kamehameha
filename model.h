#ifndef MODEL3D_H
#define MODEL3D_H
#include <QMap>
#include <vector>

#include "triangle.h"
#include "material.h"

class Model
{
public:
    std::vector<Shape> shapes;
    bool smooth = false;
    QMap<QString, Material> materials;

    Model();
};

#endif // MODEL3D_H
