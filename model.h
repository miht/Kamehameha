#ifndef MODEL3D_H
#define MODEL3D_H
#include <QMap>
#include <vector>

#include "material.h"
#include "object.h"

class Model
{
public:
    QMap<QString, Material> materials;
    std::vector<Object> objects;

    Model();
};

#endif // MODEL3D_H
