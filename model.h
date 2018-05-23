#ifndef MODEL3D_H
#define MODEL3D_H
#include <QMap>
#include <vector>

#include "fbxsdk.h"

#include "material.h"
#include "object.h"
#include "kdnode.h"
#include "scenemetadata.h"

class Model
{
public:
    int numberOfVertices, numberOfEdges, numberOfFaces;
    SceneMetadata metadata;
    QMap<const char*, Material> materials;
    std::vector<Object> objects;

    KDNode* root;

    Model();
};

#endif // MODEL3D_H
