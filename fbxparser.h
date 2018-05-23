#ifndef FBXPARSER_H
#define FBXPARSER_H

#include "fbxsdk.h"
#include "scene.h"
#include "geometry.h"
#include <QMap>

class FbxParser
{
public:
    FbxParser();

    static void process(FbxNode *node, Scene *scene);
    static void processMaterials(FbxMesh *mesh, QMap<const char*, Material> &materials);
    static void processMesh(const FbxMesh *mesh, std::vector<Face*> &faces);
    static void processLight(const FbxLight *light, std::vector<Light> &lights);

};

#endif // FBXPARSER_H
