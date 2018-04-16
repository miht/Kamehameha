#ifndef MODEL3D_H
#define MODEL3D_H
#include <QMap>
#include <vector>

#include "material.h"
#include "object.h"
#include "kdnode.h"

class Model
{
public:
    QMap<QString, Material> materials;
    std::vector<Object> objects;
    KDNode* root;

    Model();
};

#endif // MODEL3D_H
